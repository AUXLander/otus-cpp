#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <cassert>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std::string_view_literals;

struct point
{
    float x;
    float y;
    float color;

    point() = default;

    point(float x, float y) :
        x(x), y(y)
    {;}

    void draw(float offset_x, float offset_y) const
    {
        std::cout << "draw some point" << std::endl;
    }
};

class primitive
{
public:
    enum type_e : size_t
    {
        LINE_STRIP,
        RECTANGLE,
        TRIANGLE,
        CIRCLE
    };

private:
    type_e __type;

protected:
    primitive(type_e type) :
        __type(type)
    {
        std::cout << "create " << name() << std::endl;
    }

    float x;
    float y;

    virtual void save_primitive_data(std::fstream& out) const = 0;
    virtual void load_primitive_data(std::fstream& out) = 0;

public:
    virtual void draw(float primitive_x, float primitive_y) const = 0;

    static void save(const primitive& p, std::fstream& out)
    {
        std::cout << "write type to header: " << static_cast<size_t>(p.__type) << std::endl;

        // out.write(reinterpret_cast<const char*>(&p.__type), sizeof(p.__type));

        p.save_primitive_data(out);
    }

    static primitive& load(std::fstream& out);

    void set_position(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    auto get_position() const
    {
        return std::make_tuple(x, y);
    }

    const std::string_view& name() const
    {
        switch (__type)
        {
            case type_e::CIRCLE: return "CIRCLE"sv;
            case type_e::LINE_STRIP: return "LINE_STRIP"sv;
            case type_e::RECTANGLE: return "RECTANGLE"sv;
            case type_e::TRIANGLE: return "TRIANGLE"sv;

            default:
                assert(false && "Unknown primitive type");
        }

        return "UNKNOWN"sv;
    }
};

class line_strip_primitive : public primitive
{
protected:
    std::vector<point> __points;

    line_strip_primitive(type_e type, size_t initial_points_count) :
        primitive(type), __points(initial_points_count)
    {;}

public:
    void draw(float primitive_x, float primitive_y) const override
    {
        std::cout << "draw points linestrip primitive: " << this->name() << std::endl;

        for (const auto& point : __points)
        {
            point.draw(primitive_x, primitive_y);
        }
    }

    void save_primitive_data(std::fstream& out) const override
    {
        std::cout << "save each point of: " << this->name() << std::endl;
    }

    void load_primitive_data(std::fstream& out) override
    {
        std::cout << "load each point of: " << this->name() << std::endl;
    }

    const std::vector<point>& get_points() const
    {
        return __points;
    }
};

struct line_strip : public line_strip_primitive
{
    line_strip() : line_strip_primitive(type_e::LINE_STRIP, 2U) { ; }

    template<typename ...Args>
    size_t emplace_point(Args&& ...args)
    {
        std::cout << "add new point to primitive" << std::endl;

        const size_t new_point_index = __points.size();

        __points.emplace_back(std::forward<Args>(args)...);

        return new_point_index;
    }

    bool erase_point(size_t index)
    {
        std::cout << "erase point from primitive" << std::endl;

        assert(index < __points.size());

        if (auto it = std::next(__points.begin(), index); it != __points.end())
        {
            __points.erase(it);

            return true;
        }

        return false;
    }
};

struct rectangle : public line_strip_primitive
{
    rectangle() : line_strip_primitive(type_e::RECTANGLE, 4U) { ; }
};

struct triangle : public line_strip_primitive
{
    triangle() : line_strip_primitive(type_e::TRIANGLE, 3U) { ; }
};

struct circle : public primitive
{
    float radius;

    circle(float radius = 0) :
        primitive(type_e::CIRCLE), radius(radius)
    {;}

    void draw(float center_x, float center_y) const override
    {
        std::cout << "draw a " << name() << " of radius " << radius << std::endl;
    }

    void save_primitive_data(std::fstream& out) const override
    {
        std::cout << "save position and radius of: " << this->name() << std::endl;
    }

    void load_primitive_data(std::fstream& out) override
    {
        std::cout << "load position and radius of: " << this->name() << std::endl;
    }
};

primitive& primitive::load(std::fstream& out)
{
    std::cout << "read type of primitive from header " << std::endl;

    // DEMO!
    size_t primitive_type = rand() % 4;

    primitive* p = nullptr;

    // out.read(reinterpret_cast<char*>(&primitive_type), sizeof(primitive_type));

    switch (primitive_type)
    {
        case type_e::CIRCLE: p = new circle(); break;
        case type_e::LINE_STRIP: p = new line_strip(); break;
        case type_e::RECTANGLE: p = new rectangle(); break;
        case type_e::TRIANGLE: p = new triangle(); break;

        default:
            assert(false && "Unknown primitive type");
    }

    p->load_primitive_data(out);

    return *p;
}


// class to save or load data of primitives
// provides methods to put bytes into file or fill primitive by data
class file_t
{
    std::fstream fstream;

public:

    template<typename ...Args>
    file_t(Args&& ...args) :
        fstream(std::forward<Args>(args)...)
    {
        std::cout << "open file" << std::endl;
    }

    template<typename Tlambda>
    void save_to_file(Tlambda lambda)
    {
        // if (fstream.is_open())
        {
            lambda(fstream);
        }
    }

    template<typename Tlambda>
    void load_from_file(Tlambda lambda)
    {
        // if (fstream.is_open())
        {
            size_t count = 3;

            // fstream.read(reinterpret_cast<char*>(&count), sizeof(count));

            while (count--)
            {
                lambda(primitive::load(fstream));
            }
        }
    }

    ~file_t()
    {
        std::cout << "close file" << std::endl;

        if (fstream.is_open())
        {
            fstream.close();
        }
    }
};

// namespace of graphic user interface
namespace gui
{
    // any generated by gui events
    struct event
    {
        enum class type_e
        {
            MOUSE_CLICK,
            FILE_EXPORT,
            FILE_IMPORT
        };

        type_e type;

        event(type_e type) : 
            type(type)
        {;}

        virtual const char* name() = 0;
    };

    struct mouse_event : event
    {
        primitive::type_e primitive_type;

        mouse_event(type_e type, primitive::type_e primitive_type) :
            event(type), primitive_type(primitive_type)
        {;}

        const char* name() override
        {
            return "mouse event";
        }
    };

    struct file_event : event
    {
        std::string path;

        file_event(type_e type, std::string path) :
            event(type), path(path)
        {;}

        const char* filepath() const
        {
            return path.c_str();
        }

        const char* name() override
        {
            return "file event";
        }
    };
};


// own and manage all primitives.
// waiting for an events to process them.
// for example waiting for mouse click to emplace primitive
struct canvas
{
    std::vector<std::unique_ptr<primitive>> primitives;

    canvas()
    {
        std::cout << "create an emprty canvas to draw!" << std::endl;
    }

    void process(const gui::event& e)
    {
        switch (e.type)
        {
            // очень условный кейс
            case gui::event::type_e::MOUSE_CLICK:
            {
                const auto mouse_e = dynamic_cast<const gui::mouse_event&>(e);

                primitive* p = nullptr;

                switch (mouse_e.primitive_type)
                {
                    case primitive::type_e::CIRCLE: p = new circle(); break;
                    case primitive::type_e::LINE_STRIP: p = new line_strip(); break;
                    case primitive::type_e::RECTANGLE: p = new rectangle(); break;
                    case primitive::type_e::TRIANGLE: p = new triangle(); break;

                    default:
                        assert(false && "Unknown primitive type");
                }

                primitives.emplace_back(std::unique_ptr<primitive>(p));
            };
            break;

            case gui::event::type_e::FILE_IMPORT:
            {
                const auto file_e = dynamic_cast<const gui::file_event&>(e);

                file_t(file_e.filepath(), std::ios_base::binary | std::ios_base::out)
                    .load_from_file(
                        [&](primitive& p)
                        {
                            primitives.emplace_back(std::unique_ptr<primitive>(&p));
                        }
                );
            };
            break;

            case gui::event::type_e::FILE_EXPORT:
            {
                const auto file_e = dynamic_cast<const gui::file_event&>(e);

                file_t(file_e.filepath(), std::ios_base::binary | std::ios_base::in)
                    .save_to_file(
                        [&](std::fstream& in)
                        {
                            for (const auto& p : primitives)
                            {
                                primitive::save(*p, in);
                            }
                        }
                );
            };
            break;
        }
    }

    void draw() const
    {
        for (const auto& p : primitives)
        {
            auto [x, y] = p->get_position();

            p->draw(x, y);
        }
    }
};

int main()
{
    // start to draw!
    {
        canvas cnv;

        auto event1 = gui::mouse_event(gui::event::type_e::MOUSE_CLICK, primitive::type_e::TRIANGLE);
        auto event2 = gui::mouse_event(gui::event::type_e::MOUSE_CLICK, primitive::type_e::RECTANGLE);
        auto event3 = gui::mouse_event(gui::event::type_e::MOUSE_CLICK, primitive::type_e::CIRCLE);

        cnv.process(event1);
        cnv.process(event2);
        cnv.process(event3);

        cnv.draw();

        auto event = gui::file_event(gui::event::type_e::FILE_EXPORT, "/SQL/denikolaev/vector-image");

        cnv.process(event);
    }

    // load image from file!
    {
        canvas cnv;

        auto event = gui::file_event(gui::event::type_e::FILE_IMPORT, "/SQL/denikolaev/vector-image");

        cnv.process(event);

        cnv.draw();
    }
}
