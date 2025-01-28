to get up and running

place the plugins folder within your unreal engine project directory

You may need to delete the intermediate and binaries folders if you're running a version other than UE 5.5

Once you have that, and your project is up and running:

1. make a new level
2. place the ProceduralChunkManager object from your plugins directory in your project into your level. You can check out this video for some context checks: https://www.youtube.com/watch?v=wKAlnUTZGh8&t=105s and/or https://youtu.be/x8Ur4TJGaDQ?si=u3wJTj8IUPRCkTo6
3. configure the options.


Notes:

The adding of individual blueprint actors within the chunk actors is not yet developed. As you can see though, the framework is laid out in case you wish to continue development in C++.

Hope this helps someone!
