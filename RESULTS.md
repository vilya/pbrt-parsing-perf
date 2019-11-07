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

| Filename         |  pbrt-parser |     minipbrt |      Speedup |
| :--------------- | -----------: | -----------: | -----------: |
| pbrt/island.pbrt |     5346.921 |      168.613 |       31.71x |



Results for all scenes in pbrt-v3-scenes
----------------------------------------

- Source: https://www.pbrt.org/scenes-v3.html
- The disk cache was prewarmed by running minipbrt and reading (but not
  parsing) any .ply files
- pbrt-parser runs first after the prewarming step, minipbrt runs second.

| Filename                                         |  pbrt-parser |     minipbrt |      Speedup |
| :----------------------------------------------- | -----------: | -----------: | -----------: |
| barcelona-pavilion/pavilion-day.pbrt             |        5.216 |        0.901 |        5.79x |
| barcelona-pavilion/pavilion-night.pbrt           |        5.236 |        0.932 |        5.62x |
| bathroom/bathroom.pbrt                           |        0.334 |        0.058 |        5.72x |
| bmw-m6/bmw-m6.pbrt                               |        0.584 |        0.102 |        5.75x |
| breakfast/breakfast.pbrt                         |       failed |        0.043 |            - |
| breakfast/breakfast-lamps.pbrt                   |        0.365 |        0.041 |        8.83x |
| breakfast/f16-8a.pbrt                            |        0.365 |        0.043 |        8.49x |
| breakfast/f16-8b.pbrt                            |        0.341 |        0.042 |        8.15x |
| buddha-fractal/buddha-fractal.pbrt               |        0.996 |        0.030 |       32.90x |
| bunny-fur/f3-15.pbrt                             |       56.807 |        2.018 |       28.15x |
| caustic-glass/f16-11a.pbrt                       |       failed |        0.014 |            - |
| caustic-glass/f16-11b.pbrt                       |       failed |        0.015 |            - |
| caustic-glass/f16-9a.pbrt                        |       failed |        0.017 |            - |
| caustic-glass/f16-9b.pbrt                        |       failed |        0.014 |            - |
| caustic-glass/f16-9c.pbrt                        |       failed |        0.018 |            - |
| caustic-glass/glass.pbrt                         |       failed |        0.017 |            - |
| chopper-titan/chopper-titan.pbrt                 |        3.361 |        0.643 |        5.23x |
| cloud/cloud.pbrt                                 |        0.334 |        0.012 |       27.11x |
| cloud/f15-4a.pbrt                                |        0.345 |        0.012 |       27.92x |
| cloud/f15-4b.pbrt                                |        0.363 |        0.012 |       30.03x |
| cloud/f15-4c.pbrt                                |        0.348 |        0.013 |       26.49x |
| cloud/smoke.pbrt                                 |        0.343 |        0.013 |       25.44x |
| coffee-splash/f15-5.pbrt                         |        0.245 |        0.033 |        7.48x |
| coffee-splash/splash.pbrt                        |        0.236 |        0.030 |        8.00x |
| contemporary-bathroom/contemporary-bathroom.pbrt |        0.515 |        0.144 |        3.57x |
| crown/crown.pbrt                                 |        2.757 |        0.502 |        5.49x |
| dambreak/dambreak0.pbrt                          |        0.973 |        0.145 |        6.71x |
| dambreak/dambreak1.pbrt                          |        1.621 |        0.249 |        6.51x |
| dragon/f11-13.pbrt                               |        3.199 |        0.905 |        3.53x |
| dragon/f11-14.pbrt                               |        3.167 |        0.902 |        3.51x |
| dragon/f14-3.pbrt                                |        1.600 |        0.451 |        3.55x |
| dragon/f14-5.pbrt                                |        3.188 |        0.888 |        3.59x |
| dragon/f15-13.pbrt                               |        3.269 |        0.900 |        3.63x |
| dragon/f8-10.pbrt                                |        1.637 |        0.460 |        3.56x |
| dragon/f8-14a.pbrt                               |        1.576 |        0.470 |        3.35x |
| dragon/f8-14b.pbrt                               |        1.575 |        0.462 |        3.41x |
| dragon/f8-21a.pbrt                               |        1.610 |        0.452 |        3.56x |
| dragon/f8-21b.pbrt                               |        1.576 |        0.453 |        3.48x |
| dragon/f8-24.pbrt                                |        3.116 |        0.887 |        3.51x |
| dragon/f8-4a.pbrt                                |        1.530 |        0.453 |        3.37x |
| dragon/f8-4b.pbrt                                |        1.536 |        0.466 |        3.30x |
| dragon/f9-3.pbrt                                 |        1.568 |        0.472 |        3.32x |
| dragon/f9-4.pbrt                                 |        1.607 |        0.467 |        3.44x |
| ecosys/ecosys.pbrt                               |        1.603 |        0.245 |        6.54x |
| figures/f10-1ac.pbrt                             |        0.000 |        0.000 |        1.07x |
| figures/f10-1b.pbrt                              |        0.000 |        0.000 |        1.09x |
| figures/f11-15.pbrt                              |        0.000 |        0.000 |        1.27x |
| figures/f3-18.pbrt                               |       failed |        0.000 |            - |
| figures/f7-19a.pbrt                              |        0.000 |        0.000 |        0.66x |
| figures/f7-19b.pbrt                              |        0.000 |        0.000 |        0.73x |
| figures/f7-19c.pbrt                              |        0.000 |        0.000 |        0.78x |
| figures/f7-30a.pbrt                              |        0.000 |        0.001 |        0.61x |
| figures/f7-30b.pbrt                              |        0.000 |        0.000 |        0.66x |
| figures/f7-30c.pbrt                              |        0.000 |        0.000 |        0.71x |
| figures/f7-34a.pbrt                              |        0.000 |        0.000 |        0.72x |
| figures/f7-34b.pbrt                              |        0.000 |        0.000 |        0.68x |
| figures/f7-34c.pbrt                              |        0.000 |        0.000 |        0.81x |
| figures/f8-22.pbrt                               |        0.001 |        0.001 |        2.20x |
| ganesha/f3-11.pbrt                               |        4.478 |        0.703 |        6.37x |
| ganesha/ganesha.pbrt                             |        4.308 |        0.708 |        6.08x |
| hair/curly-hair.pbrt                             |      157.695 |        7.150 |       22.06x |
| hair/sphere-hairblock.pbrt                       |        0.371 |        0.014 |       26.45x |
| hair/straight-hair.pbrt                          |       61.855 |        2.349 |       26.33x |
| head/f9-5.pbrt                                   |        0.044 |        0.017 |        2.62x |
| head/head.pbrt                                   |        0.051 |        0.013 |        3.82x |
| killeroos/killeroo-gold.pbrt                     |        0.047 |        0.006 |        7.46x |
| killeroos/killeroo-moving.pbrt                   |        0.096 |        0.010 |        9.29x |
| killeroos/killeroo-simple.pbrt                   |        0.088 |        0.010 |        9.18x |
| landscape/f4-1.pbrt                              |       55.547 |        8.040 |        6.91x |
| landscape/f6-13.pbrt                             |       54.772 |        7.989 |        6.86x |
| landscape/f6-14.pbrt                             |       53.499 |        7.889 |        6.78x |
| landscape/view-0.pbrt                            |       51.290 |        7.928 |        6.47x |
| landscape/view-1.pbrt                            |       50.759 |        7.868 |        6.45x |
| landscape/view-2.pbrt                            |       50.142 |        7.820 |        6.41x |
| landscape/view-3.pbrt                            |       49.482 |        7.861 |        6.29x |
| landscape/view-4.pbrt                            |       48.405 |        7.845 |        6.17x |
| lte-orb/lte-orb-roughglass.pbrt                  |        1.277 |        0.050 |       25.45x |
| lte-orb/lte-orb-silver.pbrt                      |        1.262 |        0.049 |       25.87x |
| measure-one/frame120.pbrt                        |       14.888 |        5.253 |        2.83x |
| measure-one/frame180.pbrt                        |       15.113 |        6.701 |        2.26x |
| measure-one/frame210.pbrt                        |       15.115 |        6.560 |        2.30x |
| measure-one/frame25.pbrt                         |       15.474 |        6.524 |        2.37x |
| measure-one/frame300.pbrt                        |       15.258 |        6.586 |        2.32x |
| measure-one/frame35.pbrt                         |       15.119 |        6.885 |        2.20x |
| measure-one/frame380.pbrt                        |       15.021 |        6.615 |        2.27x |
| measure-one/frame52.pbrt                         |       15.183 |        6.551 |        2.32x |
| measure-one/frame85.pbrt                         |       14.947 |        6.665 |        2.24x |
| pbrt-book/book.pbrt                              |        0.078 |        0.019 |        4.21x |
| sanmiguel/f10-8.pbrt                             |       22.456 |        2.884 |        7.79x |
| sanmiguel/f16-21a.pbrt                           |       22.426 |        2.761 |        8.12x |
| sanmiguel/f16-21b.pbrt                           |       22.375 |        2.836 |        7.89x |
| sanmiguel/f16-21c.pbrt                           |       22.381 |        2.905 |        7.71x |
| sanmiguel/f6-17.pbrt                             |       22.819 |        2.733 |        8.35x |
| sanmiguel/f6-25.pbrt                             |       22.402 |        3.271 |        6.85x |
| sanmiguel/sanmiguel.pbrt                         |       22.677 |        2.802 |        8.09x |
| sanmiguel/sanmiguel_cam1.pbrt                    |       22.237 |        2.692 |        8.26x |
| sanmiguel/sanmiguel_cam14.pbrt                   |       16.307 |        2.620 |        6.22x |
| sanmiguel/sanmiguel_cam15.pbrt                   |       14.290 |        2.639 |        5.41x |
| sanmiguel/sanmiguel_cam18.pbrt                   |       23.377 |        2.782 |        8.40x |
| sanmiguel/sanmiguel_cam20.pbrt                   |       22.137 |        2.830 |        7.82x |
| sanmiguel/sanmiguel_cam25.pbrt                   |       22.219 |        2.711 |        8.19x |
| sanmiguel/sanmiguel_cam3.pbrt                    |       22.498 |        2.693 |        8.35x |
| sanmiguel/sanmiguel_cam4.pbrt                    |       22.170 |        2.837 |        7.81x |
| simple/anim-bluespheres.pbrt                     |        0.000 |        0.000 |        0.84x |
| simple/buddha.pbrt                               |        1.379 |        0.192 |        7.17x |
| simple/bump-sphere.pbrt                          |        0.001 |        0.001 |        0.98x |
| simple/caustic-proj.pbrt                         |        0.000 |        0.001 |        0.48x |
| simple/dof-dragons.pbrt                          |       12.892 |        0.331 |       39.00x |
| simple/miscquads.pbrt                            |        0.001 |        0.001 |        1.22x |
| simple/room-mlt.pbrt                             |       failed |        0.012 |            - |
| simple/room-path.pbrt                            |       failed |        0.012 |            - |
| simple/room-sppm.pbrt                            |       failed |        0.012 |            - |
| simple/spheres-differentials-texfilt.pbrt        |        0.000 |        0.000 |        0.85x |
| simple/spotfog.pbrt                              |       failed |        0.000 |            - |
| simple/teapot-area-light.pbrt                    |       failed |        0.005 |            - |
| simple/teapot-metal.pbrt                         |        0.105 |        0.004 |       26.16x |
| smoke-plume/plume-084.pbrt                       |        7.951 |        0.260 |       30.62x |
| smoke-plume/plume-184.pbrt                       |        8.956 |        0.318 |       28.13x |
| smoke-plume/plume-284.pbrt                       |        8.987 |        0.317 |       28.36x |
| sportscar/f12-19a.pbrt                           |        8.093 |        1.323 |        6.12x |
| sportscar/f12-19b.pbrt                           |        8.080 |        1.351 |        5.98x |
| sportscar/f12-20a.pbrt                           |        8.146 |        1.326 |        6.14x |
| sportscar/f12-20b.pbrt                           |        8.081 |        1.356 |        5.96x |
| sportscar/f7-37a.pbrt                            |        8.169 |        1.328 |        6.15x |
| sportscar/f7-37b.pbrt                            |        8.189 |        1.373 |        5.96x |
| sportscar/sportscar.pbrt                         |        8.105 |        1.308 |        6.20x |
| sssdragon/dragon_10.pbrt                         |       failed |        1.085 |            - |
| sssdragon/dragon_250.pbrt                        |       failed |        1.047 |            - |
| sssdragon/dragon_50.pbrt                         |       failed |        1.088 |            - |
| sssdragon/f15-7.pbrt                             |       failed |        1.051 |            - |
| structuresynth/arcsphere.pbrt                    |        0.296 |        0.013 |       23.69x |
| structuresynth/ballpile.pbrt                     |        0.070 |        0.004 |       15.86x |
| structuresynth/metal.pbrt                        |        0.151 |        0.007 |       21.92x |
| structuresynth/microcity.pbrt                    |        0.554 |        0.023 |       23.95x |
| transparent-machines/frame1266.pbrt              |        6.143 |        2.398 |        2.56x |
| transparent-machines/frame542.pbrt               |        2.395 |        0.931 |        2.57x |
| transparent-machines/frame675.pbrt               |        2.966 |        1.159 |        2.56x |
| transparent-machines/frame812.pbrt               |        3.821 |        1.577 |        2.42x |
| transparent-machines/frame888.pbrt               |        4.517 |        1.815 |        2.49x |
| tt/tt.pbrt                                       |        1.319 |        0.532 |        2.48x |
| veach-bidir/bidir.pbrt                           |        0.008 |        0.010 |        0.82x |
| veach-mis/mis.pbrt                               |        0.001 |        0.004 |        0.27x |
| villa/f16-20a.pbrt                               |        4.245 |        1.386 |        3.06x |
| villa/f16-20b.pbrt                               |        4.312 |        1.403 |        3.07x |
| villa/f16-20c.pbrt                               |        4.302 |        1.426 |        3.02x |
| villa/villa-daylight.pbrt                        |        4.307 |        1.420 |        3.03x |
| villa/villa-lights-on.pbrt                       |       failed |        1.391 |            - |
| villa/villa-photons.pbrt                         |       failed |        1.373 |            - |
| volume-caustic/caustic.pbrt                      |       failed |        0.000 |            - |
| volume-caustic/f16-22a.pbrt                      |       failed |        0.001 |            - |
| volume-caustic/f16-22b.pbrt                      |       failed |        0.000 |            - |
| vw-van/vw-van.pbrt                               |        2.641 |        0.670 |        3.94x |
| white-room/whiteroom-daytime.pbrt                |        1.095 |        0.444 |        2.47x |
| white-room/whiteroom-night.pbrt                  |        1.309 |        0.517 |        2.53x |
| yeahright/yeahright.pbrt                         |        0.172 |        0.069 |        2.49x |


Results for all scenes in Benedikt Bitterli's collection
--------------------------------------------------------

- Source: https://benedikt-bitterli.me/resources/
- The disk cache was prewarmed by running minipbrt and reading (but not
  parsing) any .ply files
- pbrt-parser runs first after the prewarming step, minipbrt runs second.

| Filename                        |  pbrt-parser |     minipbrt |      Speedup |
| :------------------------------ | -----------: | -----------: | -----------: |
| ./bathroom/scene.pbrt           |        0.612 |        0.164 |        3.73x |
| ./bathroom2/scene.pbrt          |        1.105 |        0.204 |        5.40x |
| ./bedroom/scene.pbrt            |        1.556 |        0.269 |        5.79x |
| ./car/scene.pbrt                |        0.683 |        0.121 |        5.63x |
| ./car2/scene.pbrt               |        0.953 |        0.159 |        6.00x |
| ./classroom/scene.pbrt          |        0.104 |        0.021 |        4.95x |
| ./coffee/scene.pbrt             |        0.187 |        0.032 |        5.80x |
| ./cornell-box/scene.pbrt        |        0.001 |        0.000 |        2.96x |
| ./curly-hair/scene.pbrt         |      155.928 |        5.209 |       29.93x |
| ./dining-room/scene.pbrt        |        0.307 |        0.078 |        3.95x |
| ./dragon/scene.pbrt             |        0.971 |        0.187 |        5.20x |
| ./furball/scene.pbrt            |      180.308 |       10.075 |       17.90x |
| ./glass-of-water/scene.pbrt     |        1.029 |        0.234 |        4.40x |
| ./hair-curl/scene.pbrt          |      171.453 |        8.854 |       19.36x |
| ./house/scene.pbrt              |        6.143 |        1.924 |        3.19x |
| ./kitchen/scene.pbrt            |        5.099 |        1.809 |        2.82x |
| ./lamp/scene.pbrt               |        2.378 |        0.606 |        3.93x |
| ./living-room/scene.pbrt        |        0.570 |        0.264 |        2.16x |
| ./living-room-2/scene.pbrt      |        2.173 |        0.801 |        2.71x |
| ./living-room-3/scene.pbrt      |       42.758 |        1.855 |       23.05x |
| ./material-testball/scene.pbrt  |        0.461 |        0.094 |        4.90x |
| ./spaceship/scene.pbrt          |        1.765 |        0.605 |        2.92x |
| ./staircase/scene.pbrt          |        1.060 |        1.087 |        0.98x |
| ./staircase2/scene.pbrt         |        0.121 |        0.083 |        1.47x |
| ./straight-hair/scene.pbrt      |       77.672 |        4.018 |       19.33x |
| ./teapot/scene.pbrt             |        0.577 |        0.111 |        5.19x |
| ./teapot-full/scene.pbrt        |        0.546 |        0.138 |        3.96x |
| ./veach-ajar/scene.pbrt         |        1.452 |        0.321 |        4.53x |
| ./veach-bidir/scene.pbrt        |        0.051 |        0.042 |        1.21x |
| ./veach-mis/scene.pbrt          |        0.002 |        0.001 |        1.68x |
| ./volumetric-caustic/scene.pbrt |        0.001 |        0.001 |        1.21x |
| ./water-caustic/scene.pbrt      |        0.371 |        0.061 |        6.07x |
