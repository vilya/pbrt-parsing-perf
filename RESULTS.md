Results from pbrt-parsing-perf
==============================

Differences between minipbrt and pbrt-parser
--------------------------------------------

*pbrt-parser* and *minipbrt* have different approaches to handling PLY files:
- pbrt-parser automatically loads the files and creates a triangle mesh
  primitive from them.
- minipbrt creates a PLYMesh primitive and provides optional helper methods
  for loading it yourself.
In the interests of making the comparison as fair as possible, we're loading
all PLY files using the helper methods that minipbrt provides.

At time of writing, *minipbrt* and *pbrt-parser* support different subsets of
the PBRTv3 file format. *minipbrt* supports the entire format with the
exception of per-shape material overrides. *pbrt-parser* only supports a
commonly used subset of the format. Here's a summary:

|                    | minipbrt | pbrt-parser                                                          | 
| :----------------- | :------: | :------------------------------------------------------------------: |
| Shapes             | All      | `trianglemesh`, `curve`, `sphere`, `disk` & quad mesh (non standard) |
| Materials          | All      | All except `hair` and `kdsubsurface`                                 |
| Material overrides | No       | Yes                                                                  |
| Textures           | All      | All except `bilerp`, `dots` and `uv` (but see the note below)        |
| Lights             | All      | `infinite` and `distant` only                                        |
| Area lights        | All      | All, plus a special class for blackbody emitters                     |
| Mediums            | All      | None                                                                 |
| PLY files          | All      | `ascii` and `binary_little_endian` only.                             |

Note that *pbrt-parser* doesn't seem to store the world-to-texture transform
for 3D textures. I'm not sure whether it actually supports 3D textures at all.
It doesn't seem to store the common 2D texture parameters (`uscale`, `vscale`,
`udelta`, `vdelta`, `v1` and `v2`) either, but I may have missed something.

Both parsers convert spectral data to rgb. *minipbrt* automatically converts
blackbodies to rgb. *pbrt-parser* doesn't by default but provides a method for
doing it.


Points of interest
------------------

For many small-to-medium scenes, the biggest influence on parsing time is
whether the files are already resident in the OS's disk cache. This was
unfairly favoring whichever parsing ran second. I've attempted to fix this by
prewarming the disk cache: we do an initial simplified parsing run on each
scene before running its performance comparison.

minipbrt does particularly well relative to pbrt-parser on scenes where
there is lots of geometry stored inline in the .pbrt files. Examples include
the hair files, which have lots of inline curve primitives; and the Moana
island scene which has a lot of inline trianglemesh primitives.

There are some scenes where minipbrt is slower than pbrt-parser. I haven't
determined the cause yet, but what they have in common is that they are very
quick for *both* parsers and the difference between the two in absolute time
is very small.


Test hardware used
------------------

The laptop used for the results below was:

- Windows 10, v1903
- Intel Core i7-6700HQ CPU, 2.6 GHz
- 16 GB Dual-channel DDR4 RAM
- Samsung MZVPV512 SSD, theoretical peak read speed of 1165 MB/s

All times reported below are in seconds.


Results for all scenes in pbrt-v3-scenes
----------------------------------------

- Source: https://www.pbrt.org/scenes-v3.html
- The disk cache was prewarmed by running minipbrt and reading (but not
  parsing) any .ply files
- pbrt-parser runs first after the prewarming step, minipbrt runs second,
  threded runs third.

| Filename                                         |  pbrt-parser |     minipbrt (Speedup) |     threaded (Speedup) |
| :----------------------------------------------- | -----------: | ---------------------: | ---------------------: |
| barcelona-pavilion/pavilion-day.pbrt             |        5.475 |        0.913 (  5.99x) |        0.313 ( 17.48x) |
| barcelona-pavilion/pavilion-night.pbrt           |        5.113 |        0.885 (  5.78x) |        0.310 ( 16.49x) |
| bathroom/bathroom.pbrt                           |        0.314 |        0.059 (  5.36x) |        0.021 ( 15.31x) |
| bmw-m6/bmw-m6.pbrt                               |        0.583 |        0.101 (  5.79x) |        0.034 ( 17.11x) |
| breakfast/breakfast.pbrt                         |       failed |        0.042           |        0.016           |
| breakfast/breakfast-lamps.pbrt                   |        0.348 |        0.034 ( 10.35x) |        0.015 ( 23.45x) |
| breakfast/f16-8a.pbrt                            |        0.347 |        0.039 (  8.82x) |        0.014 ( 24.06x) |
| breakfast/f16-8b.pbrt                            |        0.344 |        0.041 (  8.46x) |        0.012 ( 29.72x) |
| buddha-fractal/buddha-fractal.pbrt               |        0.629 |        0.027 ( 23.36x) |        0.030 ( 20.78x) |
| bunny-fur/f3-15.pbrt                             |       56.029 |        2.362 ( 23.72x) |        2.412 ( 23.23x) |
| caustic-glass/f16-11a.pbrt                       |       failed |        0.016           |        0.017           |
| caustic-glass/f16-11b.pbrt                       |       failed |        0.018           |        0.020           |
| caustic-glass/f16-9a.pbrt                        |       failed |        0.017           |        0.021           |
| caustic-glass/f16-9b.pbrt                        |       failed |        0.022           |        0.021           |
| caustic-glass/f16-9c.pbrt                        |       failed |        0.017           |        0.020           |
| caustic-glass/glass.pbrt                         |       failed |        0.019           |        0.022           |
| chopper-titan/chopper-titan.pbrt                 |        3.895 |        0.826 (  4.72x) |        0.554 (  7.03x) |
| cloud/cloud.pbrt                                 |        0.334 |        0.013 ( 26.69x) |        0.015 ( 22.69x) |
| cloud/f15-4a.pbrt                                |        0.337 |        0.012 ( 28.04x) |        0.017 ( 19.87x) |
| cloud/f15-4b.pbrt                                |        0.330 |        0.013 ( 26.36x) |        0.014 ( 23.08x) |
| cloud/f15-4c.pbrt                                |        0.326 |        0.014 ( 23.47x) |        0.016 ( 20.05x) |
| cloud/smoke.pbrt                                 |        0.338 |        0.012 ( 28.10x) |        0.014 ( 23.32x) |
| coffee-splash/f15-5.pbrt                         |        0.277 |        0.040 (  6.84x) |        0.044 (  6.32x) |
| coffee-splash/splash.pbrt                        |        0.281 |        0.039 (  7.21x) |        0.044 (  6.35x) |
| contemporary-bathroom/contemporary-bathroom.pbrt |        0.592 |        0.244 (  2.42x) |        0.190 (  3.11x) |
| crown/crown.pbrt                                 |        3.061 |        0.681 (  4.50x) |        0.512 (  5.98x) |
| dambreak/dambreak0.pbrt                          |        1.040 |        0.153 (  6.79x) |        0.151 (  6.89x) |
| dambreak/dambreak1.pbrt                          |        1.741 |        0.249 (  6.99x) |        0.242 (  7.19x) |
| dragon/f11-13.pbrt                               |        3.668 |        0.904 (  4.06x) |        0.504 (  7.28x) |
| dragon/f11-14.pbrt                               |        3.801 |        0.895 (  4.25x) |        0.495 (  7.67x) |
| dragon/f14-3.pbrt                                |        1.831 |        0.448 (  4.09x) |        0.457 (  4.01x) |
| dragon/f14-5.pbrt                                |        3.823 |        0.899 (  4.25x) |        0.511 (  7.49x) |
| dragon/f15-13.pbrt                               |        3.677 |        0.906 (  4.06x) |        0.504 (  7.30x) |
| dragon/f8-10.pbrt                                |        1.841 |        0.472 (  3.90x) |        0.450 (  4.09x) |
| dragon/f8-14a.pbrt                               |        1.868 |        0.453 (  4.13x) |        0.470 (  3.98x) |
| dragon/f8-14b.pbrt                               |        1.867 |        0.452 (  4.13x) |        0.457 (  4.08x) |
| dragon/f8-21a.pbrt                               |        1.857 |        0.464 (  4.00x) |        0.455 (  4.08x) |
| dragon/f8-21b.pbrt                               |        1.874 |        0.459 (  4.08x) |        0.467 (  4.01x) |
| dragon/f8-24.pbrt                                |        3.680 |        0.898 (  4.10x) |        0.496 (  7.42x) |
| dragon/f8-4a.pbrt                                |        1.839 |        0.454 (  4.05x) |        0.449 (  4.09x) |
| dragon/f8-4b.pbrt                                |        1.834 |        0.446 (  4.11x) |        0.458 (  4.01x) |
| dragon/f9-3.pbrt                                 |        1.836 |        0.456 (  4.03x) |        0.461 (  3.99x) |
| dragon/f9-4.pbrt                                 |        1.819 |        0.454 (  4.00x) |        0.449 (  4.05x) |
| ecosys/ecosys.pbrt                               |        1.782 |        0.322 (  5.54x) |        0.205 (  8.70x) |
| figures/f10-1ac.pbrt                             |        0.000 |        0.000 (  0.92x) |        0.004 (  0.10x) |
| figures/f10-1b.pbrt                              |        0.000 |        0.000 (  0.90x) |        0.003 (  0.10x) |
| figures/f11-15.pbrt                              |        0.000 |        0.000 (  1.18x) |        0.003 (  0.13x) |
| figures/f3-18.pbrt                               |       failed |        0.000           |        0.003           |
| figures/f7-19a.pbrt                              |        0.000 |        0.000 (  0.67x) |        0.002 (  0.09x) |
| figures/f7-19b.pbrt                              |        0.000 |        0.000 (  0.64x) |        0.002 (  0.10x) |
| figures/f7-19c.pbrt                              |        0.000 |        0.000 (  0.65x) |        0.002 (  0.11x) |
| figures/f7-30a.pbrt                              |        0.000 |        0.000 (  0.70x) |        0.002 (  0.12x) |
| figures/f7-30b.pbrt                              |        0.000 |        0.000 (  0.66x) |        0.003 (  0.11x) |
| figures/f7-30c.pbrt                              |        0.000 |        0.000 (  0.70x) |        0.003 (  0.11x) |
| figures/f7-34a.pbrt                              |        0.000 |        0.001 (  0.69x) |        0.004 (  0.10x) |
| figures/f7-34b.pbrt                              |        0.000 |        0.000 (  0.68x) |        0.002 (  0.11x) |
| figures/f7-34c.pbrt                              |        0.000 |        0.000 (  0.74x) |        0.002 (  0.13x) |
| figures/f8-22.pbrt                               |        0.001 |        0.001 (  1.71x) |        0.002 (  0.53x) |
| ganesha/f3-11.pbrt                               |        5.077 |        0.690 (  7.36x) |        0.679 (  7.48x) |
| ganesha/ganesha.pbrt                             |        5.027 |        0.691 (  7.27x) |        0.687 (  7.32x) |
| hair/curly-hair.pbrt                             |      158.725 |        9.456 ( 16.79x) |       11.989 ( 13.24x) |
| hair/sphere-hairblock.pbrt                       |        0.425 |        0.018 ( 23.01x) |        0.024 ( 17.73x) |
| hair/straight-hair.pbrt                          |       84.669 |        3.803 ( 22.26x) |        4.536 ( 18.67x) |
| head/f9-5.pbrt                                   |        0.067 |        0.025 (  2.68x) |        0.024 (  2.81x) |
| head/head.pbrt                                   |        0.079 |        0.024 (  3.30x) |        0.024 (  3.29x) |
| killeroos/killeroo-gold.pbrt                     |        0.046 |        0.011 (  4.02x) |        0.013 (  3.49x) |
| killeroos/killeroo-moving.pbrt                   |        0.101 |        0.013 (  7.68x) |        0.021 (  4.80x) |
| killeroos/killeroo-simple.pbrt                   |        0.103 |        0.014 (  7.33x) |        0.016 (  6.43x) |
| landscape/f4-1.pbrt                              |       73.330 |       10.560 (  6.94x) |        7.520 (  9.75x) |
| landscape/f6-13.pbrt                             |       73.746 |       10.635 (  6.93x) |        7.724 (  9.55x) |
| landscape/f6-14.pbrt                             |       74.195 |       10.363 (  7.16x) |        7.579 (  9.79x) |
| landscape/view-0.pbrt                            |       71.660 |       10.391 (  6.90x) |        7.314 (  9.80x) |
| landscape/view-1.pbrt                            |       71.158 |       10.216 (  6.97x) |        7.258 (  9.80x) |
| landscape/view-2.pbrt                            |       70.674 |       10.182 (  6.94x) |        7.227 (  9.78x) |
| landscape/view-3.pbrt                            |       69.970 |       10.029 (  6.98x) |        7.117 (  9.83x) |
| landscape/view-4.pbrt                            |       68.524 |       10.082 (  6.80x) |        7.077 (  9.68x) |
| lte-orb/lte-orb-roughglass.pbrt                  |        1.274 |        0.062 ( 20.68x) |        0.060 ( 21.30x) |
| lte-orb/lte-orb-silver.pbrt                      |        1.277 |        0.058 ( 21.92x) |        0.068 ( 18.90x) |
| measure-one/frame120.pbrt                        |       22.201 |       11.599 (  1.91x) |       12.557 (  1.77x) |
| measure-one/frame180.pbrt                        |       21.247 |       11.689 (  1.82x) |       12.721 (  1.67x) |
| measure-one/frame210.pbrt                        |       21.433 |       11.763 (  1.82x) |       12.508 (  1.71x) |
| measure-one/frame25.pbrt                         |       21.079 |       11.778 (  1.79x) |       12.374 (  1.70x) |
| measure-one/frame300.pbrt                        |       21.215 |       11.761 (  1.80x) |       12.375 (  1.71x) |
| measure-one/frame35.pbrt                         |       20.936 |       11.607 (  1.80x) |       12.467 (  1.68x) |
| measure-one/frame380.pbrt                        |       20.919 |       11.815 (  1.77x) |       12.388 (  1.69x) |
| measure-one/frame52.pbrt                         |       20.898 |       11.829 (  1.77x) |       12.479 (  1.67x) |
| measure-one/frame85.pbrt                         |       20.835 |       11.814 (  1.76x) |       12.528 (  1.66x) |
| pbrt-book/book.pbrt                              |        0.105 |        0.025 (  4.21x) |        0.028 (  3.71x) |
| sanmiguel/f10-8.pbrt                             |       26.749 |        4.634 (  5.77x) |        4.469 (  5.99x) |
| sanmiguel/f16-21a.pbrt                           |       27.389 |        4.543 (  6.03x) |        4.466 (  6.13x) |
| sanmiguel/f16-21b.pbrt                           |       26.220 |        4.924 (  5.33x) |        4.707 (  5.57x) |
| sanmiguel/f16-21c.pbrt                           |       35.588 |        4.501 (  7.91x) |        4.446 (  8.00x) |
| sanmiguel/f6-17.pbrt                             |       31.176 |        5.332 (  5.85x) |        4.784 (  6.52x) |
| sanmiguel/f6-25.pbrt                             |       26.221 |        4.499 (  5.83x) |        4.460 (  5.88x) |
| sanmiguel/sanmiguel.pbrt                         |       26.156 |        4.476 (  5.84x) |        4.459 (  5.87x) |
| sanmiguel/sanmiguel_cam1.pbrt                    |       26.214 |        4.477 (  5.86x) |        4.430 (  5.92x) |
| sanmiguel/sanmiguel_cam14.pbrt                   |       20.409 |        4.384 (  4.66x) |        4.271 (  4.78x) |
| sanmiguel/sanmiguel_cam15.pbrt                   |       18.194 |        4.241 (  4.29x) |        4.203 (  4.33x) |
| sanmiguel/sanmiguel_cam18.pbrt                   |       26.264 |        4.469 (  5.88x) |        4.411 (  5.95x) |
| sanmiguel/sanmiguel_cam20.pbrt                   |       26.214 |        4.479 (  5.85x) |        4.424 (  5.93x) |
| sanmiguel/sanmiguel_cam25.pbrt                   |       26.482 |        4.537 (  5.84x) |        4.417 (  6.00x) |
| sanmiguel/sanmiguel_cam3.pbrt                    |       26.542 |        4.504 (  5.89x) |        4.439 (  5.98x) |
| sanmiguel/sanmiguel_cam4.pbrt                    |       26.448 |        4.519 (  5.85x) |        4.484 (  5.90x) |
| simple/anim-bluespheres.pbrt                     |        0.001 |        0.001 (  0.94x) |        0.003 (  0.21x) |
| simple/buddha.pbrt                               |        1.742 |        0.188 (  9.28x) |        0.179 (  9.71x) |
| simple/bump-sphere.pbrt                          |        0.001 |        0.001 (  0.73x) |        0.003 (  0.19x) |
| simple/caustic-proj.pbrt                         |        0.000 |        0.001 (  0.42x) |        0.003 (  0.10x) |
| simple/dof-dragons.pbrt                          |       13.453 |        0.336 ( 39.99x) |        0.211 ( 63.80x) |
| simple/miscquads.pbrt                            |        0.001 |        0.001 (  0.78x) |        0.003 (  0.23x) |
| simple/room-mlt.pbrt                             |       failed |        0.015           |        0.020           |
| simple/room-path.pbrt                            |       failed |        0.015           |        0.017           |
| simple/room-sppm.pbrt                            |       failed |        0.014           |        0.017           |
| simple/spheres-differentials-texfilt.pbrt        |        0.000 |        0.001 (  0.55x) |        0.004 (  0.12x) |
| simple/spotfog.pbrt                              |       failed |        0.001           |        0.003           |
| simple/teapot-area-light.pbrt                    |       failed |        0.005           |        0.007           |
| simple/teapot-metal.pbrt                         |        0.104 |        0.006 ( 16.30x) |        0.007 ( 15.07x) |
| smoke-plume/plume-084.pbrt                       |        7.369 |        0.263 ( 28.06x) |        0.269 ( 27.40x) |
| smoke-plume/plume-184.pbrt                       |        8.787 |        0.300 ( 29.31x) |        0.309 ( 28.47x) |
| smoke-plume/plume-284.pbrt                       |        8.873 |        0.318 ( 27.91x) |        0.316 ( 28.11x) |
| sportscar/f12-19a.pbrt                           |        9.582 |        1.691 (  5.67x) |        1.288 (  7.44x) |
| sportscar/f12-19b.pbrt                           |        9.708 |        1.854 (  5.23x) |        1.287 (  7.54x) |
| sportscar/f12-20a.pbrt                           |        9.689 |        1.714 (  5.65x) |        1.288 (  7.52x) |
| sportscar/f12-20b.pbrt                           |        9.704 |        1.721 (  5.64x) |        1.297 (  7.48x) |
| sportscar/f7-37a.pbrt                            |        9.571 |        1.719 (  5.57x) |        1.296 (  7.39x) |
| sportscar/f7-37b.pbrt                            |        9.598 |        1.720 (  5.58x) |        1.306 (  7.35x) |
| sportscar/sportscar.pbrt                         |        9.565 |        1.715 (  5.58x) |        1.280 (  7.47x) |
| sssdragon/dragon_10.pbrt                         |       failed |        1.058           |        1.068           |
| sssdragon/dragon_250.pbrt                        |       failed |        1.124           |        1.084           |
| sssdragon/dragon_50.pbrt                         |       failed |        1.065           |        1.064           |
| sssdragon/f15-7.pbrt                             |       failed |        1.066           |        1.064           |
| structuresynth/arcsphere.pbrt                    |        0.293 |        0.014 ( 20.67x) |        0.020 ( 14.68x) |
| structuresynth/ballpile.pbrt                     |        0.072 |        0.006 ( 12.28x) |        0.008 (  8.99x) |
| structuresynth/metal.pbrt                        |        0.146 |        0.008 ( 18.76x) |        0.009 ( 15.49x) |
| structuresynth/microcity.pbrt                    |        0.569 |        0.030 ( 18.79x) |        0.034 ( 16.74x) |
| transparent-machines/frame1266.pbrt              |        8.148 |        3.786 (  2.15x) |        3.871 (  2.10x) |
| transparent-machines/frame542.pbrt               |        3.200 |        1.419 (  2.26x) |        1.470 (  2.18x) |
| transparent-machines/frame675.pbrt               |        3.950 |        1.845 (  2.14x) |        1.918 (  2.06x) |
| transparent-machines/frame812.pbrt               |        5.191 |        2.461 (  2.11x) |        2.533 (  2.05x) |
| transparent-machines/frame888.pbrt               |        6.027 |        2.858 (  2.11x) |        2.969 (  2.03x) |
| tt/tt.pbrt                                       |        1.684 |        0.783 (  2.15x) |        0.794 (  2.12x) |
| veach-bidir/bidir.pbrt                           |        0.009 |        0.014 (  0.65x) |        0.016 (  0.57x) |
| veach-mis/mis.pbrt                               |        0.001 |        0.007 (  0.20x) |        0.009 (  0.15x) |
| villa/f16-20a.pbrt                               |        5.639 |        2.054 (  2.75x) |        1.994 (  2.83x) |
| villa/f16-20b.pbrt                               |        5.595 |        2.060 (  2.72x) |        1.987 (  2.82x) |
| villa/f16-20c.pbrt                               |        5.587 |        2.047 (  2.73x) |        1.971 (  2.83x) |
| villa/villa-daylight.pbrt                        |        5.579 |        2.082 (  2.68x) |        1.972 (  2.83x) |
| villa/villa-lights-on.pbrt                       |       failed |        2.029           |        1.968           |
| villa/villa-photons.pbrt                         |       failed |        2.042           |        1.994           |
| volume-caustic/caustic.pbrt                      |       failed |        0.001           |        0.003           |
| volume-caustic/f16-22a.pbrt                      |       failed |        0.001           |        0.003           |
| volume-caustic/f16-22b.pbrt                      |       failed |        0.001           |        0.004           |
| vw-van/vw-van.pbrt                               |        3.459 |        0.989 (  3.50x) |        0.967 (  3.58x) |
| white-room/whiteroom-daytime.pbrt                |        1.433 |        0.707 (  2.03x) |        0.752 (  1.91x) |
| white-room/whiteroom-night.pbrt                  |        1.452 |        0.721 (  2.01x) |        0.752 (  1.93x) |
| yeahright/yeahright.pbrt                         |        0.172 |        0.054 (  3.20x) |        0.062 (  2.76x) |


Results for all scenes in Benedikt Bitterli's collection
--------------------------------------------------------

- Source: https://benedikt-bitterli.me/resources/
- The disk cache was prewarmed by running minipbrt and reading (but not
  parsing) any .ply files
- pbrt-parser runs first after the prewarming step, minipbrt runs second, threaded runs last.


| Filename                        |  pbrt-parser |     minipbrt (Speedup) |     threaded (Speedup) |
| :------------------------------ | -----------: | ---------------------: | ---------------------: |
| ./bathroom/scene.pbrt           |        0.529 |        0.143 (  3.71x) |        0.044 ( 11.98x) |
| ./bathroom2/scene.pbrt          |        1.642 |        0.169 (  9.70x) |        0.078 ( 20.96x) |
| ./bedroom/scene.pbrt            |        1.353 |        0.246 (  5.51x) |        0.186 (  7.28x) |
| ./car/scene.pbrt                |        0.608 |        0.111 (  5.47x) |        0.055 ( 11.00x) |
| ./car2/scene.pbrt               |        0.829 |        0.147 (  5.63x) |        0.050 ( 16.58x) |
| ./classroom/scene.pbrt          |        0.098 |        0.018 (  5.40x) |        0.008 ( 12.34x) |
| ./coffee/scene.pbrt             |        0.169 |        0.030 (  5.55x) |        0.014 ( 12.23x) |
| ./cornell-box/scene.pbrt        |        0.001 |        0.000 (  3.57x) |        0.003 (  0.48x) |
| ./curly-hair/scene.pbrt         |      160.376 |        6.722 ( 23.86x) |        8.438 ( 19.01x) |
| ./dining-room/scene.pbrt        |        0.495 |        0.209 (  2.37x) |        0.214 (  2.31x) |
| ./dragon/scene.pbrt             |        1.949 |        0.246 (  7.92x) |        0.181 ( 10.77x) |
| ./furball/scene.pbrt            |      179.320 |       11.731 ( 15.29x) |       14.717 ( 12.18x) |
| ./glass-of-water/scene.pbrt     |        1.490 |        0.402 (  3.71x) |        0.366 (  4.07x) |
| ./hair-curl/scene.pbrt          |      186.714 |       13.662 ( 13.67x) |       16.269 ( 11.48x) |
| ./house/scene.pbrt              |        8.991 |        3.294 (  2.73x) |        3.284 (  2.74x) |
| ./kitchen/scene.pbrt            |        7.494 |        3.215 (  2.33x) |        3.186 (  2.35x) |
| ./lamp/scene.pbrt               |        3.345 |        0.922 (  3.63x) |        0.939 (  3.56x) |
| ./living-room/scene.pbrt        |        0.854 |        0.461 (  1.85x) |        0.470 (  1.82x) |
| ./living-room-2/scene.pbrt      |        3.168 |        1.379 (  2.30x) |        1.395 (  2.27x) |
| ./living-room-3/scene.pbrt      |       50.233 |        2.987 ( 16.82x) |        3.580 ( 14.03x) |
| ./material-testball/scene.pbrt  |        0.670 |        0.142 (  4.70x) |        0.145 (  4.62x) |
| ./spaceship/scene.pbrt          |        2.708 |        1.017 (  2.66x) |        1.030 (  2.63x) |
| ./staircase/scene.pbrt          |        1.516 |        1.816 (  0.83x) |        1.913 (  0.79x) |
| ./staircase2/scene.pbrt         |        0.158 |        0.134 (  1.18x) |        0.138 (  1.15x) |
| ./straight-hair/scene.pbrt      |       90.288 |        5.906 ( 15.29x) |        8.577 ( 10.53x) |
| ./teapot/scene.pbrt             |        1.026 |        0.146 (  7.01x) |        0.145 (  7.08x) |
| ./teapot-full/scene.pbrt        |        0.827 |        0.179 (  4.63x) |        0.190 (  4.36x) |
| ./veach-ajar/scene.pbrt         |        2.336 |        0.478 (  4.89x) |        0.474 (  4.93x) |
| ./veach-bidir/scene.pbrt        |        0.086 |        0.065 (  1.33x) |        0.076 (  1.13x) |
| ./veach-mis/scene.pbrt          |        0.002 |        0.001 (  1.36x) |        0.004 (  0.43x) |
| ./volumetric-caustic/scene.pbrt |        0.001 |        0.001 (  0.75x) |        0.004 (  0.28x) |
| ./water-caustic/scene.pbrt      |        0.586 |        0.092 (  6.39x) |        0.096 (  6.13x) |


Results for Disney's Moana island scene
---------------------------------------

Source: https://www.technology.disneyanimation.com/islandscene
- The disk cache was not prewarmed for these comparisons. The data set is so
  large that whether the data was previously present in the disk cache has 
  very little impact on the overall times.
- pbrt-parser ran first, minipbrt ran second.
- The scene does not use any PLY files so the threaded parser should be
  roughly the same speed as minipbrt. I did not bother to run it.


| Filename         |  pbrt-parser |     minipbrt |      Speedup |
| :--------------- | -----------: | -----------: | -----------: |
| pbrt/island.pbrt |     5346.921 |      168.613 |       31.71x |
