Results from pbrt-parsing-perf
==============================

Differences between minipbrt and pbrt-parser
--------------------------------------------

*pbrt-parser* and *minipbrt* have different approaches to handling PLY files:

- pbrt-parser transparently loads them into a triangle mesh primitive.
- minipbrt creates a PLYMesh primitive and provides optional helper methods
  for loading it yourself.

In the interests of making the comparison as fair as possible, we're loading
all PLY files using the helper methods that minipbrt provides.

At time of writing, *minipbrt* and *pbrt-parser* support different subsets of
the PBRTv3 file format. *minipbrt* supports the entire format with the
exception of per-shape material overrides. *pbrt-parser* only supports the 
more commonly used parts of the format:
- `trianglemesh`, `curve`, `sphere` and `disk` shapes, plus a non-standard 
  `quadmesh` shape.
- All materials except `hair` and `kdsubsurface`.
- All texture types except `bilerp`, `dots` and `uv`.
  - I'm not sure whether *pbrt-parser* supports 3D textures - it doesn't seem 
    to store the world-to-texture transform.
  - Likewise, it doesn't seem to store the common 2D texture parameters
    (`uscale`, `vscale`, `udelta`, `vdelta`, `v1` and `v2`).
- `infinite` and `distant` lights only.
- `diffuse` area lights (the only type there is), with a special class for
  diffuse area lights with an emission spectrum defined as a blackbody.
- No mediums.

Both parsers convert spectral data to rgb. *minipbrt* automatically converts
blackbodies to rgb. *pbrt-parser* doesn't by default but provides a method for
doing it).


Points of interest
------------------

minipbrt does particularly well relative to pbrt-parser on scenes where
there is lots of geometry stored inline in the .pbrt files. Examples include
the hair files, which have lots of inline curve primitives; and the Moana
island scene which has a lot of inline trianglemesh primitives.

There are some scenes where minipbrt is slower than pbrt-parser. I haven't
determined the cause yet, but what they have in common is that they are very
quick for *both* parsers.

For many small-to-medium scenes, the biggest influence on parsing time is
whether the files are already resident in the OS's disk cache. This was
unfairly favoring whichever parsing ran second. I've attempted to fix this by
prewarming the disk cache: we do an initial simplified parsing run on each
scene before running its performance comparison.


Test hardware used
------------------

The laptop used for the results below was:

- Windows 10, v1903
- Intel Core i7-6700HQ CPU, 2.6 GHz
- 16 GB Dual-channel DDR4 RAM
- Samsung MZVPV512 SSD, theoretical peak read speed of 1165 MB/s

All times reported below are in seconds.


Results for Disney's Moana island scene
---------------------------------------

Source: https://www.technology.disneyanimation.com/islandscene
- The disk cache was not prewarmed for these comparisons. The data set is so
  large that whether the data was previously present in the disk cache has 
  very little impact on the overall times.
- pbrt-parser ran first, minipbrt ran second.  

| Filename                                         |  pbrt-parser |     minipbrt |      Speedup |
| :----------------------------------------------- | -----------: | -----------: | -----------: |
| island/pbrt/island.pbrt                          |     5387.676 |      183.550 |       29.35x |



Results for all scenes in pbrt-v3-scenes
----------------------------------------

- Source: https://www.pbrt.org/scenes-v3.html
- The disk cache was prewarmed by running minipbrt and reading (but not
  parsing) any .ply files
- pbrt-parser runs first after the prewarming step, minipbrt runs second.

| Filename                                         |  pbrt-parser |     minipbrt |      Speedup |
| :----------------------------------------------- | -----------: | -----------: | -----------: |
| barcelona-pavilion/pavilion-day.pbrt             |        5.518 |        0.924 |        5.97x |
| barcelona-pavilion/pavilion-night.pbrt           |        5.934 |        1.116 |        5.32x |
| bathroom/bathroom.pbrt                           |        0.441 |        0.064 |        6.94x |
| bmw-m6/bmw-m6.pbrt                               |        0.670 |        0.104 |        6.46x |
| breakfast/breakfast.pbrt                         |       failed |        0.047 |            - |
| breakfast/breakfast-lamps.pbrt                   |        0.383 |        0.045 |        8.55x |
| breakfast/f16-8a.pbrt                            |        0.359 |        0.039 |        9.12x |
| breakfast/f16-8b.pbrt                            |        0.354 |        0.037 |        9.58x |
| buddha-fractal/buddha-fractal.pbrt               |        0.702 |        0.029 |       24.57x |
| bunny-fur/f3-15.pbrt                             |       58.931 |        2.196 |       26.83x |
| caustic-glass/f16-11a.pbrt                       |       failed |        0.014 |            - |
| caustic-glass/f16-11b.pbrt                       |       failed |        0.014 |            - |
| caustic-glass/f16-9a.pbrt                        |       failed |        0.015 |            - |
| caustic-glass/f16-9b.pbrt                        |       failed |        0.016 |            - |
| caustic-glass/f16-9c.pbrt                        |       failed |        0.016 |            - |
| caustic-glass/glass.pbrt                         |       failed |        0.016 |            - |
| chopper-titan/chopper-titan.pbrt                 |        3.468 |        0.717 |        4.84x |
| cloud/cloud.pbrt                                 |        0.331 |        0.012 |       28.66x |
| cloud/f15-4a.pbrt                                |        0.327 |        0.012 |       28.36x |
| cloud/f15-4b.pbrt                                |        0.322 |        0.013 |       25.48x |
| cloud/f15-4c.pbrt                                |        0.340 |        0.012 |       27.88x |
| cloud/smoke.pbrt                                 |        0.341 |        0.012 |       28.32x |
| coffee-splash/f15-5.pbrt                         |        0.269 |        0.042 |        6.46x |
| coffee-splash/splash.pbrt                        |        0.264 |        0.029 |        9.00x |
| contemporary-bathroom/contemporary-bathroom.pbrt |        0.595 |        0.159 |        3.74x |
| crown/crown.pbrt                                 |        2.988 |        0.574 |        5.21x |
| dambreak/dambreak0.pbrt                          |        1.093 |        0.169 |        6.46x |
| dambreak/dambreak1.pbrt                          |        1.828 |        0.275 |        6.64x |
| dragon/f11-13.pbrt                               |        3.505 |        0.902 |        3.88x |
| dragon/f11-14.pbrt                               |        3.440 |        1.003 |        3.43x |
| dragon/f14-3.pbrt                                |        1.605 |        0.452 |        3.55x |
| dragon/f14-5.pbrt                                |        3.486 |        0.923 |        3.78x |
| dragon/f15-13.pbrt                               |        3.163 |        0.932 |        3.40x |
| dragon/f8-10.pbrt                                |        1.628 |        0.465 |        3.50x |
| dragon/f8-14a.pbrt                               |        1.555 |        0.452 |        3.44x |
| dragon/f8-14b.pbrt                               |        1.578 |        0.490 |        3.22x |
| dragon/f8-21a.pbrt                               |        1.626 |        0.469 |        3.47x |
| dragon/f8-21b.pbrt                               |        1.739 |        0.494 |        3.52x |
| dragon/f8-24.pbrt                                |        3.597 |        0.918 |        3.92x |
| dragon/f8-4a.pbrt                                |        1.552 |        0.476 |        3.26x |
| dragon/f8-4b.pbrt                                |        1.572 |        0.440 |        3.57x |
| dragon/f9-3.pbrt                                 |        1.542 |        0.440 |        3.50x |
| dragon/f9-4.pbrt                                 |        1.627 |        0.466 |        3.49x |
| ecosys/ecosys.pbrt                               |        1.671 |        0.251 |        6.65x |
| figures/f10-1ac.pbrt                             |        0.000 |        0.000 |        1.16x |
| figures/f10-1b.pbrt                              |        0.000 |        0.000 |        1.10x |
| figures/f11-15.pbrt                              |        0.000 |        0.000 |        1.27x |
| figures/f3-18.pbrt                               |       failed |        0.000 |            - |
| figures/f7-19a.pbrt                              |        0.000 |        0.000 |        0.79x |
| figures/f7-19b.pbrt                              |        0.000 |        0.000 |        0.77x |
| figures/f7-19c.pbrt                              |        0.000 |        0.000 |        0.75x |
| figures/f7-30a.pbrt                              |        0.000 |        0.000 |        0.80x |
| figures/f7-30b.pbrt                              |        0.000 |        0.000 |        0.65x |
| figures/f7-30c.pbrt                              |        0.000 |        0.000 |        0.68x |
| figures/f7-34a.pbrt                              |        0.000 |        0.000 |        0.72x |
| figures/f7-34b.pbrt                              |        0.000 |        0.000 |        0.75x |
| figures/f7-34c.pbrt                              |        0.000 |        0.000 |        0.75x |
| figures/f8-22.pbrt                               |        0.001 |        0.001 |        2.14x |
| ganesha/f3-11.pbrt                               |        4.575 |        0.688 |        6.65x |
| ganesha/ganesha.pbrt                             |        4.450 |        0.739 |        6.02x |
| hair/curly-hair.pbrt                             |      168.856 |        6.935 |       24.35x |
| hair/sphere-hairblock.pbrt                       |        0.421 |        0.015 |       28.36x |
| hair/straight-hair.pbrt                          |      202.995 |        2.587 |       78.45x |
| head/f9-5.pbrt                                   |        0.041 |        0.014 |        2.90x |
| head/head.pbrt                                   |        0.048 |        0.012 |        3.97x |
| killeroos/killeroo-gold.pbrt                     |        0.050 |        0.006 |        7.87x |
| killeroos/killeroo-moving.pbrt                   |        0.088 |        0.008 |       11.58x |
| killeroos/killeroo-simple.pbrt                   |        0.088 |        0.007 |       13.45x |
| landscape/f4-1.pbrt                              |       52.641 |        7.787 |        6.76x |
| landscape/f6-13.pbrt                             |       52.273 |        8.333 |        6.27x |
| landscape/f6-14.pbrt                             |       52.740 |        8.032 |        6.57x |
| landscape/view-0.pbrt                            |       50.649 |        7.713 |        6.57x |
| landscape/view-1.pbrt                            |       49.869 |        8.841 |        5.64x |
| landscape/view-2.pbrt                            |       47.772 |        7.819 |        6.11x |
| landscape/view-3.pbrt                            |       48.663 |        7.653 |        6.36x |
| landscape/view-4.pbrt                            |       47.562 |        7.686 |        6.19x |
| lte-orb/lte-orb-roughglass.pbrt                  |        1.292 |        0.050 |       26.08x |
| lte-orb/lte-orb-silver.pbrt                      |        1.325 |        0.045 |       29.22x |
| measure-one/frame120.pbrt                        |       13.095 |        3.842 |        3.41x |
| measure-one/frame180.pbrt                        |       14.391 |        5.048 |        2.85x |
| measure-one/frame210.pbrt                        |       13.919 |        5.484 |        2.54x |
| measure-one/frame25.pbrt                         |       14.207 |        7.387 |        1.92x |
| measure-one/frame300.pbrt                        |       16.530 |        6.306 |        2.62x |
| measure-one/frame35.pbrt                         |       15.710 |        6.304 |        2.49x |
| measure-one/frame380.pbrt                        |       14.110 |        5.461 |        2.58x |
| measure-one/frame52.pbrt                         |       14.705 |        5.736 |        2.56x |
| measure-one/frame85.pbrt                         |       16.938 |        6.244 |        2.71x |
| pbrt-book/book.pbrt                              |        0.076 |        0.017 |        4.62x |
| sanmiguel/f10-8.pbrt                             |       23.997 |        2.777 |        8.64x |
| sanmiguel/f16-21a.pbrt                           |       24.443 |        2.508 |        9.75x |
| sanmiguel/f16-21b.pbrt                           |       22.261 |        2.454 |        9.07x |
| sanmiguel/f16-21c.pbrt                           |       21.509 |        2.406 |        8.94x |
| sanmiguel/f6-17.pbrt                             |       21.087 |        2.341 |        9.01x |
| sanmiguel/f6-25.pbrt                             |       21.562 |        2.417 |        8.92x |
| sanmiguel/sanmiguel.pbrt                         |       21.001 |        2.376 |        8.84x |
| sanmiguel/sanmiguel_cam1.pbrt                    |       21.132 |        2.477 |        8.53x |
| sanmiguel/sanmiguel_cam14.pbrt                   |       16.812 |        2.539 |        6.62x |
| sanmiguel/sanmiguel_cam15.pbrt                   |       14.177 |        2.271 |        6.24x |
| sanmiguel/sanmiguel_cam18.pbrt                   |       20.870 |        2.357 |        8.86x |
| sanmiguel/sanmiguel_cam20.pbrt                   |       20.901 |        2.438 |        8.57x |
| sanmiguel/sanmiguel_cam25.pbrt                   |       21.366 |        2.532 |        8.44x |
| sanmiguel/sanmiguel_cam3.pbrt                    |       21.012 |        2.361 |        8.90x |
| sanmiguel/sanmiguel_cam4.pbrt                    |       21.017 |        2.458 |        8.55x |
| simple/anim-bluespheres.pbrt                     |        0.000 |        0.001 |        0.88x |
| simple/buddha.pbrt                               |        1.457 |        0.193 |        7.53x |
| simple/bump-sphere.pbrt                          |        0.000 |        0.001 |        0.91x |
| simple/caustic-proj.pbrt                         |        0.000 |        0.001 |        0.62x |
| simple/dof-dragons.pbrt                          |       13.171 |        0.313 |       42.02x |
| simple/miscquads.pbrt                            |        0.001 |        0.001 |        1.23x |
| simple/room-mlt.pbrt                             |       failed |        0.012 |            - |
| simple/room-path.pbrt                            |       failed |        0.010 |            - |
| simple/room-sppm.pbrt                            |       failed |        0.012 |            - |
| simple/spheres-differentials-texfilt.pbrt        |        0.000 |        0.001 |        0.69x |
| simple/spotfog.pbrt                              |       failed |        0.001 |            - |
| simple/teapot-area-light.pbrt                    |       failed |        0.004 |            - |
| simple/teapot-metal.pbrt                         |        0.103 |        0.004 |       24.70x |
| smoke-plume/plume-084.pbrt                       |        7.382 |        0.261 |       28.31x |
| smoke-plume/plume-184.pbrt                       |        8.955 |        0.303 |       29.58x |
| smoke-plume/plume-284.pbrt                       |        8.968 |        0.309 |       29.01x |
| sportscar/f12-19a.pbrt                           |        7.933 |        1.231 |        6.44x |
| sportscar/f12-19b.pbrt                           |        7.765 |        1.192 |        6.52x |
| sportscar/f12-20a.pbrt                           |        7.780 |        1.205 |        6.46x |
| sportscar/f12-20b.pbrt                           |        7.972 |        1.208 |        6.60x |
| sportscar/f7-37a.pbrt                            |        7.627 |        1.193 |        6.39x |
| sportscar/f7-37b.pbrt                            |        7.533 |        1.238 |        6.09x |
| sportscar/sportscar.pbrt                         |        7.717 |        1.247 |        6.19x |
| sssdragon/dragon_10.pbrt                         |       failed |        1.085 |            - |
| sssdragon/dragon_250.pbrt                        |       failed |        1.067 |            - |
| sssdragon/dragon_50.pbrt                         |       failed |        1.069 |            - |
| sssdragon/f15-7.pbrt                             |       failed |        1.089 |            - |
| structuresynth/arcsphere.pbrt                    |        0.299 |        0.013 |       23.24x |
| structuresynth/ballpile.pbrt                     |        0.069 |        0.005 |       13.71x |
| structuresynth/metal.pbrt                        |        0.149 |        0.008 |       19.81x |
| structuresynth/microcity.pbrt                    |        0.554 |        0.021 |       26.91x |
| transparent-machines/frame1266.pbrt              |        5.431 |        2.079 |        2.61x |
| transparent-machines/frame542.pbrt               |        2.214 |        0.783 |        2.83x |
| transparent-machines/frame675.pbrt               |        2.859 |        1.017 |        2.81x |
| transparent-machines/frame812.pbrt               |        3.463 |        1.375 |        2.52x |
| transparent-machines/frame888.pbrt               |        4.020 |        1.589 |        2.53x |
| tt/tt.pbrt                                       |        1.203 |        0.472 |        2.55x |
| veach-bidir/bidir.pbrt                           |        0.007 |        0.008 |        0.85x |
| veach-mis/mis.pbrt                               |        0.001 |        0.005 |        0.23x |
| villa/f16-20a.pbrt                               |        3.943 |        1.223 |        3.23x |
| villa/f16-20b.pbrt                               |        3.969 |        1.247 |        3.18x |
| villa/f16-20c.pbrt                               |        4.002 |        1.271 |        3.15x |
| villa/villa-daylight.pbrt                        |        3.937 |        1.250 |        3.15x |
| villa/villa-lights-on.pbrt                       |       failed |        1.261 |            - |
| villa/villa-photons.pbrt                         |       failed |        1.251 |            - |
| volume-caustic/caustic.pbrt                      |       failed |        0.000 |            - |
| volume-caustic/f16-22a.pbrt                      |       failed |        0.001 |            - |
| volume-caustic/f16-22b.pbrt                      |       failed |        0.000 |            - |
| vw-van/vw-van.pbrt                               |        2.420 |        0.612 |        3.96x |
| white-room/whiteroom-daytime.pbrt                |        0.971 |        0.400 |        2.43x |
| white-room/whiteroom-night.pbrt                  |        0.977 |        0.396 |        2.47x |
| yeahright/yeahright.pbrt                         |        0.145 |        0.041 |        3.56x |



Results for all scenes in Benedikt Bitterli's collection
--------------------------------------------------------

- Source: https://benedikt-bitterli.me/resources/
- The disk cache was prewarmed by running minipbrt and reading (but not
  parsing) any .ply files
- pbrt-parser runs first after the prewarming step, minipbrt runs second.

| Filename                        |  pbrt-parser |     minipbrt |      Speedup |
| :------------------------------ | -----------: | -----------: | -----------: |
| ./bathroom/scene.pbrt           |        0.527 |        0.143 |        3.68x |
| ./bathroom2/scene.pbrt          |        0.976 |        0.169 |        5.79x |
| ./bedroom/scene.pbrt            |        1.353 |        0.230 |        5.90x |
| ./car/scene.pbrt                |        0.623 |        0.107 |        5.81x |
| ./car2/scene.pbrt               |        0.834 |        0.135 |        6.18x |
| ./classroom/scene.pbrt          |        0.096 |        0.020 |        4.79x |
| ./coffee/scene.pbrt             |        0.177 |        0.029 |        6.13x |
| ./cornell-box/scene.pbrt        |        0.002 |        0.000 |        3.66x |
| ./curly-hair/scene.pbrt         |      157.214 |        5.319 |       29.56x |
| ./dining-room/scene.pbrt        |        0.330 |        0.085 |        3.87x |
| ./dragon/scene.pbrt             |        0.958 |        0.190 |        5.05x |
| ./furball/scene.pbrt            |      180.514 |        8.300 |       21.75x |
| ./glass-of-water/scene.pbrt     |        0.981 |        0.221 |        4.44x |
| ./hair-curl/scene.pbrt          |      167.404 |        9.044 |       18.51x |
| ./house/scene.pbrt              |        6.128 |        1.994 |        3.07x |
| ./kitchen/scene.pbrt            |        5.188 |        1.756 |        2.95x |
| ./lamp/scene.pbrt               |        2.302 |        0.557 |        4.13x |
| ./living-room/scene.pbrt        |        0.605 |        0.273 |        2.22x |
| ./living-room-2/scene.pbrt      |        2.165 |        0.805 |        2.69x |
| ./living-room-3/scene.pbrt      |       41.457 |        2.025 |       20.47x |
| ./material-testball/scene.pbrt  |        0.513 |        0.103 |        4.97x |
| ./spaceship/scene.pbrt          |        1.765 |        0.597 |        2.96x |
| ./staircase/scene.pbrt          |        1.057 |        1.073 |        0.98x |
| ./staircase2/scene.pbrt         |        0.116 |        0.081 |        1.43x |
| ./straight-hair/scene.pbrt      |       81.019 |        3.694 |       21.93x |
| ./teapot/scene.pbrt             |        0.468 |        0.104 |        4.49x |
| ./teapot-full/scene.pbrt        |        0.490 |        0.120 |        4.08x |
| ./veach-ajar/scene.pbrt         |        1.369 |        0.313 |        4.38x |
| ./veach-bidir/scene.pbrt        |        0.054 |        0.042 |        1.30x |
| ./veach-mis/scene.pbrt          |        0.002 |        0.001 |        1.77x |
| ./volumetric-caustic/scene.pbrt |        0.001 |        0.001 |        1.01x |
| ./water-caustic/scene.pbrt      |        0.375 |        0.065 |        5.73x |

