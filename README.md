# coding-companion
## Clangd
CMake will create a *compile_commands.json* file in the build folder. In order for clangd to recognize things like include folders, it needs to know where the file is. clangd will look in the root folder of the project. You can either copy the *compile_commands.json* file over or create a simlink. 
```bash simlink command 
ln -s ***path/to/file/compile_commands.json*** ***path/to/root/of/project/compile_commands.json***
```
