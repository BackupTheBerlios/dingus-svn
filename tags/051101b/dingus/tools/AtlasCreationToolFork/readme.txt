This is a fork of nVidia's AtlasCreationTool.
Original Atlas Creation Tool can be found at http://developer.nvidia.com/object/texture_atlas_tools.html
Atlas Creation Tool is (C) NVIDIA Corporation

This fork contains only the Atlas Creation Tool portion and doesn't include
the atlas viewer nor other stuff.

Currently the fork is based on 2.0 (1.0.0826.1700) version.

Changes are:

* Supports non-pow-2 input textures. Now, it packs them much better than with
  original packer, but this isn't that good for mipmaps. However, I use it
  to pack arbitrary non-pow-2 (for UI) images into atlases, and mipmaps
  aren't an issue for my case.

* Removed dependency on D3D sample framework. Now it's really just a simple
  command line application.
