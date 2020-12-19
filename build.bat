pushd bin
cl  ..\raggy.cpp /Zi /Od /I ..\include /link /debug /subsystem:console /LIBPATH:..\lib SDL2.lib SDL2main.lib shell32.lib SDL2_mixer.lib SDL2_ttf.lib vld.lib
popd