Results from pbrt-parsing-perf
==============================

Notes
-----

pbrt-parser and minipbrt have different approaches to handling PLY files:
- pbrt-parser transparently loads them into a triangle mesh primitive.
- minipbrt creates a PLYMesh primitive and provides optional helper methods
  for loading it yourself.
In the interests of making the comparison as fair as possible, we're loading
all PLY files using the helper methods that minipbrt provides.

minipbrt supports the entire PBRTv3 file format, wheeras pbrt-parser only
supports a (useful, common) subset of it. The subset supported by pbrt-parser
is:
- Shapes: trianglemesh, curve, sphere and disk only (plus its own
  non-standard, but very useful, quadmesh type).
- Materials: everything except hair and kdsubsurface.
- Textures: everything except bilerp, dots and uv. Not sure whether it supports
  3D textures - it doesn't seem to store the world-to-texture transform. Likewise
  it doesn't store any of the common 2D texture parameters (uscale, vscale, udelta,
  vdelta, v1 and v2).
- Lights: infinite and distant only.
- Area lights: diffuse, with a special type for diffuse defined as a blackbody.
- Mediums: none.

Both parsers convert spectral data to rgb.

minipbrt automatically converts blackbodies to rgb. pbrt-parser does not (but
provides a method for doing it).

Times for each parser are in seconds.


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
- Samsung MZVPV512 SSD, 1165 MB/s read speed.


Results for Disney's Moana island scene
---------------------------------------

Source:

- The disk cache was not prewarmed for these comparisons. The data set is so
  large that whether the data was previously present in the disk cache has 
  very little impact on the overall times.
- pbrt-parser ran first, minipbrt ran second.  

| Filename                                         |  pbrt-parser |     minipbrt |      Speedup |
| :----------------------------------------------- | -----------: | -----------: | -----------: |
| island/pbrt/island.pbrt                          |     5387.676 |      177.581 |       30.34x |



Results for all scenes in pbrt-v3-scenes
----------------------------------------

- Source: https://www.pbrt.org/scenes-v3.html
- The disk cache was prewarmed by running minipbrt and reading (but not
  parsing) any .ply files
- pbrt-parser runs first after the prewarming step, minipbrt runs second.
- minipbrt is loading all ply files and extracting vertex data from them, but
  does not yet extract face data (i.e. vertex indices) so it's doing slightly
  less work than pbrt-parser on each .ply file.

| Filename                                         |  pbrt-parser |     minipbrt |      Speedup |
| :----------------------------------------------- | -----------: | -----------: | -----------: |
| barcelona-pavilion/pavilion-day.pbrt             |        5.485 |        0.255 |       21.54x |
| barcelona-pavilion/pavilion-night.pbrt           |        5.813 |        0.258 |       22.52x |
| bathroom/bathroom.pbrt                           |        0.378 |        0.015 |       25.30x |
| bmw-m6/bmw-m6.pbrt                               |        0.673 |        0.027 |       25.39x |
| breakfast/breakfast.pbrt                         |       failed |        0.013 |            - |
| breakfast/breakfast-lamps.pbrt                   |        0.402 |        0.012 |       34.90x |
| breakfast/f16-8a.pbrt                            |        0.398 |        0.014 |       28.28x |
| breakfast/f16-8b.pbrt                            |        0.398 |        0.012 |       32.37x |
| buddha-fractal/buddha-fractal.pbrt               |        0.758 |        0.032 |       23.72x |
| bunny-fur/f3-15.pbrt                             |       61.142 |        2.311 |       26.46x |
| caustic-glass/f16-11a.pbrt                       |       failed |        0.003 |            - |
| caustic-glass/f16-11b.pbrt                       |       failed |        0.002 |            - |
| caustic-glass/f16-9a.pbrt                        |       failed |        0.003 |            - |
| caustic-glass/f16-9b.pbrt                        |       failed |        0.004 |            - |
| caustic-glass/f16-9c.pbrt                        |       failed |        0.002 |            - |
| caustic-glass/glass.pbrt                         |       failed |        0.002 |            - |
| chopper-titan/chopper-titan.pbrt                 |        3.789 |        0.107 |       35.44x |
| cloud/cloud.pbrt                                 |        0.680 |        0.012 |       55.11x |
| cloud/f15-4a.pbrt                                |        0.354 |        0.013 |       27.04x |
| cloud/f15-4b.pbrt                                |        0.354 |        0.013 |       26.91x |
| cloud/f15-4c.pbrt                                |        0.357 |        0.013 |       28.02x |
| cloud/smoke.pbrt                                 |        0.350 |        0.012 |       28.25x |
| coffee-splash/f15-5.pbrt                         |        0.257 |        0.006 |       40.21x |
| coffee-splash/splash.pbrt                        |        0.256 |        0.006 |       40.76x |
| contemporary-bathroom/contemporary-bathroom.pbrt |        0.534 |        0.092 |        5.79x |
| crown/crown.pbrt                                 |        2.756 |        0.131 |       21.03x |
| dambreak/dambreak0.pbrt                          |        1.007 |        0.068 |       14.85x |
| dambreak/dambreak1.pbrt                          |        1.723 |        0.112 |       15.42x |
| dragon/f11-13.pbrt                               |        3.323 |        0.033 |      100.02x |
| dragon/f11-14.pbrt                               |        3.256 |        0.033 |       97.64x |
| dragon/f14-3.pbrt                                |        1.649 |        0.018 |       92.56x |
| dragon/f14-5.pbrt                                |        3.695 |        0.036 |      102.67x |
| dragon/f15-13.pbrt                               |        3.651 |        0.036 |      102.60x |
| dragon/f8-10.pbrt                                |        1.832 |        0.018 |      101.90x |
| dragon/f8-14a.pbrt                               |        1.796 |        0.017 |      103.20x |
| dragon/f8-14b.pbrt                               |        1.855 |        0.018 |      103.20x |
| dragon/f8-21a.pbrt                               |        1.793 |        0.018 |      102.36x |
| dragon/f8-21b.pbrt                               |        1.803 |        0.017 |      104.83x |
| dragon/f8-24.pbrt                                |        3.394 |        0.034 |       99.62x |
| dragon/f8-4a.pbrt                                |        1.756 |        0.018 |       97.53x |
| dragon/f8-4b.pbrt                                |        1.852 |        0.017 |      106.74x |
| dragon/f9-3.pbrt                                 |        1.917 |        0.018 |      107.65x |
| dragon/f9-4.pbrt                                 |        1.746 |        0.017 |      101.02x |
| ecosys/ecosys.pbrt                               |        1.741 |        0.125 |       13.95x |
| figures/f10-1ac.pbrt                             |        0.000 |        0.000 |        0.99x |
| figures/f10-1b.pbrt                              |        0.001 |        0.001 |        1.23x |
| figures/f11-15.pbrt                              |        0.000 |        0.000 |        0.97x |
| figures/f3-18.pbrt                               |       failed |        0.000 |            - |
| figures/f7-19a.pbrt                              |        0.000 |        0.000 |        0.76x |
| figures/f7-19b.pbrt                              |        0.000 |        0.000 |        0.65x |
| figures/f7-19c.pbrt                              |        0.000 |        0.000 |        0.74x |
| figures/f7-30a.pbrt                              |        0.000 |        0.000 |        0.92x |
| figures/f7-30b.pbrt                              |        0.000 |        0.000 |        0.94x |
| figures/f7-30c.pbrt                              |        0.000 |        0.000 |        0.97x |
| figures/f7-34a.pbrt                              |        0.000 |        0.000 |        1.02x |
| figures/f7-34b.pbrt                              |        0.000 |        0.000 |        1.03x |
| figures/f7-34c.pbrt                              |        0.000 |        0.000 |        0.76x |
| figures/f8-22.pbrt                               |        0.001 |        0.001 |        2.17x |
| ganesha/f3-11.pbrt                               |        5.486 |        0.122 |       44.89x |
| ganesha/ganesha.pbrt                             |        4.881 |        0.156 |       31.28x |
| hair/curly-hair.pbrt                             |      154.700 |        6.674 |       23.18x |
| hair/sphere-hairblock.pbrt                       |        0.357 |        0.013 |       27.82x |
| hair/straight-hair.pbrt                          |       60.865 |        2.451 |       24.83x |
| head/f9-5.pbrt                                   |        0.043 |        0.003 |       13.08x |
| head/head.pbrt                                   |        0.047 |        0.003 |       13.85x |
| killeroos/killeroo-gold.pbrt                     |        0.043 |        0.006 |        6.94x |
| killeroos/killeroo-moving.pbrt                   |        0.088 |        0.006 |       13.80x |
| killeroos/killeroo-simple.pbrt                   |        0.089 |        0.008 |       11.79x |
| landscape/f4-1.pbrt                              |       52.614 |        1.667 |       31.56x |
| landscape/f6-13.pbrt                             |       51.043 |        1.779 |       28.69x |
| landscape/f6-14.pbrt                             |       49.551 |        1.672 |       29.64x |
| landscape/view-0.pbrt                            |       48.526 |        1.654 |       29.34x |
| landscape/view-1.pbrt                            |       47.638 |        1.660 |       28.70x |
| landscape/view-2.pbrt                            |       46.801 |        1.645 |       28.45x |
| landscape/view-3.pbrt                            |       47.825 |        1.601 |       29.88x |
| landscape/view-4.pbrt                            |       45.867 |        1.603 |       28.61x |
| lte-orb/lte-orb-roughglass.pbrt                  |        1.229 |        0.041 |       30.15x |
| lte-orb/lte-orb-silver.pbrt                      |        1.244 |        0.040 |       31.40x |
| measure-one/frame120.pbrt                        |       13.215 |        3.588 |        3.68x |
| measure-one/frame180.pbrt                        |       14.185 |        4.223 |        3.36x |
| measure-one/frame210.pbrt                        |       14.164 |        4.228 |        3.35x |
| measure-one/frame25.pbrt                         |       14.280 |        4.220 |        3.38x |
| measure-one/frame300.pbrt                        |       14.066 |        4.306 |        3.27x |
| measure-one/frame35.pbrt                         |       14.120 |        4.427 |        3.19x |
| measure-one/frame380.pbrt                        |       14.215 |        4.372 |        3.25x |
| measure-one/frame52.pbrt                         |       14.018 |        4.354 |        3.22x |
| measure-one/frame85.pbrt                         |       13.940 |        4.294 |        3.25x |
| pbrt-book/book.pbrt                              |        0.070 |        0.007 |       10.13x |
| sanmiguel/f10-8.pbrt                             |       21.234 |        1.536 |       13.82x |
| sanmiguel/f16-21a.pbrt                           |       21.385 |        1.604 |       13.33x |
| sanmiguel/f16-21b.pbrt                           |       21.367 |        1.522 |       14.04x |
| sanmiguel/f16-21c.pbrt                           |       21.262 |        1.509 |       14.09x |
| sanmiguel/f6-17.pbrt                             |       21.272 |        1.603 |       13.27x |
| sanmiguel/f6-25.pbrt                             |       21.469 |        1.507 |       14.24x |
| sanmiguel/sanmiguel.pbrt                         |       21.199 |        1.591 |       13.32x |
| sanmiguel/sanmiguel_cam1.pbrt                    |       21.402 |        1.543 |       13.87x |
| sanmiguel/sanmiguel_cam14.pbrt                   |       15.709 |        1.423 |       11.04x |
| sanmiguel/sanmiguel_cam15.pbrt                   |       13.451 |        1.425 |        9.44x |
| sanmiguel/sanmiguel_cam18.pbrt                   |       21.348 |        1.519 |       14.06x |
| sanmiguel/sanmiguel_cam20.pbrt                   |       21.337 |        1.594 |       13.38x |
| sanmiguel/sanmiguel_cam25.pbrt                   |       21.270 |        1.498 |       14.20x |
| sanmiguel/sanmiguel_cam3.pbrt                    |       21.168 |        1.515 |       13.98x |
| sanmiguel/sanmiguel_cam4.pbrt                    |       21.167 |        1.588 |       13.33x |
| simple/anim-bluespheres.pbrt                     |        0.000 |        0.001 |        0.79x |
| simple/buddha.pbrt                               |        1.325 |        0.010 |      134.94x |
| simple/bump-sphere.pbrt                          |        0.001 |        0.001 |        1.02x |
| simple/caustic-proj.pbrt                         |        0.000 |        0.001 |        0.58x |
| simple/dof-dragons.pbrt                          |       12.696 |        0.082 |      154.69x |
| simple/miscquads.pbrt                            |        0.001 |        0.001 |        1.18x |
| simple/room-mlt.pbrt                             |       failed |        0.011 |            - |
| simple/room-path.pbrt                            |       failed |        0.011 |            - |
| simple/room-sppm.pbrt                            |       failed |        0.012 |            - |
| simple/spheres-differentials-texfilt.pbrt        |        0.001 |        0.001 |        0.71x |
| simple/spotfog.pbrt                              |       failed |        0.001 |            - |
| simple/teapot-area-light.pbrt                    |       failed |        0.004 |            - |
| simple/teapot-metal.pbrt                         |        0.099 |        0.004 |       23.93x |
| smoke-plume/plume-084.pbrt                       |        7.306 |        0.263 |       27.75x |
| smoke-plume/plume-184.pbrt                       |        8.678 |        0.294 |       29.47x |
| smoke-plume/plume-284.pbrt                       |        9.061 |        0.312 |       29.03x |
| sportscar/f12-19a.pbrt                           |        7.874 |        0.240 |       32.84x |
| sportscar/f12-19b.pbrt                           |        7.868 |        0.247 |       31.90x |
| sportscar/f12-20a.pbrt                           |        7.943 |        0.248 |       31.98x |
| sportscar/f12-20b.pbrt                           |        8.530 |        0.248 |       34.41x |
| sportscar/f7-37a.pbrt                            |        7.856 |        0.254 |       30.93x |
| sportscar/f7-37b.pbrt                            |        7.951 |        0.244 |       32.63x |
| sportscar/sportscar.pbrt                         |        7.954 |        0.246 |       32.35x |
| sssdragon/dragon_10.pbrt                         |       failed |        0.081 |            - |
| sssdragon/dragon_250.pbrt                        |       failed |        0.085 |            - |
| sssdragon/dragon_50.pbrt                         |       failed |        0.090 |            - |
| sssdragon/f15-7.pbrt                             |       failed |        0.083 |            - |
| structuresynth/arcsphere.pbrt                    |        0.283 |        0.013 |       21.97x |
| structuresynth/ballpile.pbrt                     |        0.066 |        0.004 |       15.99x |
| structuresynth/metal.pbrt                        |        0.141 |        0.007 |       20.01x |
| structuresynth/microcity.pbrt                    |        0.533 |        0.020 |       26.15x |
| transparent-machines/frame1266.pbrt              |        5.639 |        0.863 |        6.53x |
| transparent-machines/frame542.pbrt               |        2.256 |        0.281 |        8.04x |
| transparent-machines/frame675.pbrt               |        2.735 |        0.439 |        6.22x |
| transparent-machines/frame812.pbrt               |        3.579 |        0.536 |        6.67x |
| transparent-machines/frame888.pbrt               |        4.157 |        0.666 |        6.25x |
| tt/tt.pbrt                                       |        1.210 |        0.307 |        3.94x |
| veach-bidir/bidir.pbrt                           |        0.007 |        0.007 |        1.03x |
| veach-mis/mis.pbrt                               |        0.001 |        0.004 |        0.29x |
| villa/f16-20a.pbrt                               |        4.059 |        0.382 |       10.61x |
| villa/f16-20b.pbrt                               |        4.046 |        0.396 |       10.23x |
| villa/f16-20c.pbrt                               |        4.029 |        0.375 |       10.75x |
| villa/villa-daylight.pbrt                        |        4.060 |        0.381 |       10.65x |
| villa/villa-lights-on.pbrt                       |       failed |        0.377 |            - |
| villa/villa-photons.pbrt                         |       failed |        0.379 |            - |
| volume-caustic/caustic.pbrt                      |       failed |        0.001 |            - |
| volume-caustic/f16-22a.pbrt                      |       failed |        0.000 |            - |
| volume-caustic/f16-22b.pbrt                      |       failed |        0.001 |            - |
| vw-van/vw-van.pbrt                               |        2.474 |        0.184 |       13.44x |
| white-room/whiteroom-daytime.pbrt                |        0.978 |        0.159 |        6.15x |
| white-room/whiteroom-night.pbrt                  |        0.969 |        0.170 |        5.70x |
| yeahright/yeahright.pbrt                         |        0.149 |        0.003 |       46.03x |



Results for all scenes in Benedikt Bitterli's collection
--------------------------------------------------------

- Source: https://benedikt-bitterli.me/resources/
- The disk cache was prewarmed by running minipbrt and reading (but not
  parsing) any .ply files
- pbrt-parser runs first after the prewarming step, minipbrt runs second.
- minipbrt is loading all ply files and extracting vertex data from them, but
  does not yet extract face data (i.e. vertex indices) so it's doing slightly
  less work than pbrt-parser on each .ply file.

| Filename                        |  pbrt-parser |     minipbrt |      Speedup |
| :------------------------------ | -----------: | -----------: | -----------: |
| ./bathroom/scene.pbrt           |        0.529 |        0.062 |        8.47x |
| ./bathroom2/scene.pbrt          |        0.942 |        0.027 |       34.86x |
| ./bedroom/scene.pbrt            |        1.299 |        0.052 |       25.08x |
| ./car/scene.pbrt                |        0.599 |        0.021 |       29.10x |
| ./car2/scene.pbrt               |        0.848 |        0.031 |       27.35x |
| ./classroom/scene.pbrt          |        0.091 |        0.008 |       11.56x |
| ./coffee/scene.pbrt             |        0.169 |        0.006 |       29.87x |
| ./cornell-box/scene.pbrt        |        0.001 |        0.000 |        3.79x |
| ./curly-hair/scene.pbrt         |      151.529 |        5.427 |       27.92x |
| ./dining-room/scene.pbrt        |        0.321 |        0.029 |       11.17x |
| ./dragon/scene.pbrt             |        1.009 |        0.028 |       35.54x |
| ./furball/scene.pbrt            |      179.041 |        9.463 |       18.92x |
| ./glass-of-water/scene.pbrt     |        1.188 |        0.064 |       18.54x |
| ./hair-curl/scene.pbrt          |      176.199 |       10.301 |       17.10x |
| ./house/scene.pbrt              |        7.921 |        0.865 |        9.16x |
| ./kitchen/scene.pbrt            |        6.523 |        0.856 |        7.62x |
| ./lamp/scene.pbrt               |        2.955 |        0.244 |       12.12x |
| ./living-room/scene.pbrt        |        0.794 |        0.177 |        4.48x |
| ./living-room-2/scene.pbrt      |        2.760 |        0.479 |        5.76x |
| ./living-room-3/scene.pbrt      |       46.965 |        1.559 |       30.13x |
| ./material-testball/scene.pbrt  |        0.608 |        0.024 |       25.57x |
| ./spaceship/scene.pbrt          |        2.332 |        0.321 |        7.26x |
| ./staircase/scene.pbrt          |        1.340 |        1.287 |        1.04x |
| ./staircase2/scene.pbrt         |        0.156 |        0.059 |        2.65x |
| ./straight-hair/scene.pbrt      |       90.174 |        4.629 |       19.48x |
| ./teapot/scene.pbrt             |        0.643 |        0.016 |       38.99x |
| ./teapot-full/scene.pbrt        |        0.667 |        0.025 |       26.71x |
| ./veach-ajar/scene.pbrt         |        1.856 |        0.092 |       20.24x |
| ./veach-bidir/scene.pbrt        |        0.064 |        0.044 |        1.46x |
| ./veach-mis/scene.pbrt          |        0.002 |        0.002 |        0.96x |
| ./volumetric-caustic/scene.pbrt |        0.001 |        0.001 |        0.87x |
| ./water-caustic/scene.pbrt      |        0.481 |        0.011 |       44.41x |

