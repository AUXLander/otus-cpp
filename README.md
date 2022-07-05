<h1> Hints to build cmake Win64 </h1>

<h2> Using vcpkg with CMake </h2>

<p> checkout and install: <a href="https://github.com/microsoft/vcpkg">vcpkg</a> <br> use inline: 

<code>
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=F:/UserData/Externals/vcpkg/scripts/buildsystems/vcpkg.cmake
</code>

</p>