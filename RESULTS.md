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

At time of writing, *minipbrt* supports the entire PBRT v3 file format while
*pbrt-parser* supports a common subset of it:

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
- Results generated using a release build of pbrt-parsing-perf from commit
  8e6750c4fb5ec2d02964c96d34e6bcfb77d89338.

| Filename                                         |  pbrt-parser |     minipbrt (Speedup) |     threaded (Speedup) |
| :----------------------------------------------- | -----------: | ---------------------: | ---------------------: |
| barcelona-pavilion/pavilion-day.pbrt             |        5.054 |        0.576 (  8.77x) |        0.227 ( 22.30x) |
| barcelona-pavilion/pavilion-night.pbrt           |        5.045 |        0.553 (  9.11x) |        0.225 ( 22.43x) |
| bathroom/bathroom.pbrt                           |        0.301 |        0.025 ( 11.93x) |        0.012 ( 25.20x) |
| bmw-m6/bmw-m6.pbrt                               |        0.566 |        0.052 ( 10.97x) |        0.025 ( 22.91x) |
| breakfast/breakfast.pbrt                         |       failed |        0.025           |        0.008           |
| breakfast/breakfast-lamps.pbrt                   |        0.334 |        0.021 ( 16.12x) |        0.007 ( 49.98x) |
| breakfast/f16-8a.pbrt                            |        0.343 |        0.021 ( 16.47x) |        0.007 ( 51.23x) |
| breakfast/f16-8b.pbrt                            |        0.329 |        0.022 ( 15.20x) |        0.007 ( 48.49x) |
| buddha-fractal/buddha-fractal.pbrt               |        0.622 |        0.027 ( 23.38x) |        0.029 ( 21.30x) |
| bunny-fur/f3-15.pbrt                             |       53.863 |        2.394 ( 22.50x) |        2.402 ( 22.43x) |
| caustic-glass/f16-11a.pbrt                       |       failed |        0.010           |        0.011           |
| caustic-glass/f16-11b.pbrt                       |       failed |        0.010           |        0.013           |
| caustic-glass/f16-9a.pbrt                        |       failed |        0.014           |        0.013           |
| caustic-glass/f16-9b.pbrt                        |       failed |        0.012           |        0.014           |
| caustic-glass/f16-9c.pbrt                        |       failed |        0.014           |        0.013           |
| caustic-glass/glass.pbrt                         |       failed |        0.014           |        0.012           |
| chopper-titan/chopper-titan.pbrt                 |        3.806 |        0.502 (  7.58x) |        0.484 (  7.86x) |
| cloud/cloud.pbrt                                 |        0.326 |        0.012 ( 27.42x) |        0.012 ( 26.42x) |
| cloud/f15-4a.pbrt                                |        0.316 |        0.012 ( 27.17x) |        0.013 ( 24.57x) |
| cloud/f15-4b.pbrt                                |        0.535 |        0.012 ( 45.55x) |        0.013 ( 39.62x) |
| cloud/f15-4c.pbrt                                |        0.323 |        0.012 ( 27.77x) |        0.013 ( 24.62x) |
| cloud/smoke.pbrt                                 |        0.321 |        0.012 ( 27.27x) |        0.013 ( 24.21x) |
| coffee-splash/f15-5.pbrt                         |        0.267 |        0.027 (  9.96x) |        0.031 (  8.53x) |
| coffee-splash/splash.pbrt                        |        0.267 |        0.027 (  9.98x) |        0.032 (  8.34x) |
| contemporary-bathroom/contemporary-bathroom.pbrt |        0.580 |        0.163 (  3.56x) |        0.178 (  3.27x) |
| crown/crown.pbrt                                 |        2.994 |        0.408 (  7.34x) |        0.460 (  6.51x) |
| dambreak/dambreak0.pbrt                          |        1.022 |        0.113 (  9.02x) |        0.114 (  8.99x) |
| dambreak/dambreak1.pbrt                          |        1.695 |        0.179 (  9.47x) |        0.180 (  9.40x) |
| dragon/f11-13.pbrt                               |        3.543 |        0.338 ( 10.49x) |        0.206 ( 17.20x) |
| dragon/f11-14.pbrt                               |        3.540 |        0.345 ( 10.25x) |        0.203 ( 17.40x) |
| dragon/f14-3.pbrt                                |        1.796 |        0.172 ( 10.47x) |        0.172 ( 10.45x) |
| dragon/f14-5.pbrt                                |        7.773 |        0.342 ( 22.71x) |        0.204 ( 38.04x) |
| dragon/f15-13.pbrt                               |        3.682 |        0.341 ( 10.79x) |        0.211 ( 17.47x) |
| dragon/f8-10.pbrt                                |        1.811 |        0.173 ( 10.44x) |        0.175 ( 10.36x) |
| dragon/f8-14a.pbrt                               |        1.829 |        0.175 ( 10.46x) |        0.174 ( 10.52x) |
| dragon/f8-14b.pbrt                               |        1.769 |        0.172 ( 10.26x) |        0.172 ( 10.26x) |
| dragon/f8-21a.pbrt                               |        1.831 |        0.172 ( 10.67x) |        0.177 ( 10.32x) |
| dragon/f8-21b.pbrt                               |        1.770 |        0.172 ( 10.27x) |        0.173 ( 10.23x) |
| dragon/f8-24.pbrt                                |        3.651 |        0.344 ( 10.62x) |        0.209 ( 17.47x) |
| dragon/f8-4a.pbrt                                |        1.775 |        0.174 ( 10.19x) |        0.174 ( 10.22x) |
| dragon/f8-4b.pbrt                                |        1.766 |        0.175 ( 10.10x) |        0.173 ( 10.18x) |
| dragon/f9-3.pbrt                                 |        1.894 |        0.174 ( 10.89x) |        0.173 ( 10.94x) |
| dragon/f9-4.pbrt                                 |        1.775 |        0.176 ( 10.07x) |        0.176 ( 10.09x) |
| ecosys/ecosys.pbrt                               |        1.748 |        0.242 (  7.22x) |        0.198 (  8.83x) |
| figures/f10-1ac.pbrt                             |        0.000 |        0.000 (  0.96x) |        0.002 (  0.17x) |
| figures/f10-1b.pbrt                              |        0.000 |        0.000 (  1.01x) |        0.002 (  0.18x) |
| figures/f11-15.pbrt                              |        0.000 |        0.000 (  1.25x) |        0.002 (  0.25x) |
| figures/f3-18.pbrt                               |       failed |        0.000           |        0.002           |
| figures/f7-19a.pbrt                              |        0.000 |        0.000 (  0.68x) |        0.002 (  0.14x) |
| figures/f7-19b.pbrt                              |        0.000 |        0.000 (  0.64x) |        0.002 (  0.12x) |
| figures/f7-19c.pbrt                              |        0.000 |        0.000 (  0.67x) |        0.002 (  0.13x) |
| figures/f7-30a.pbrt                              |        0.000 |        0.000 (  0.64x) |        0.001 (  0.19x) |
| figures/f7-30b.pbrt                              |        0.000 |        0.000 (  0.70x) |        0.002 (  0.16x) |
| figures/f7-30c.pbrt                              |        0.000 |        0.000 (  0.69x) |        0.001 (  0.20x) |
| figures/f7-34a.pbrt                              |        0.000 |        0.001 (  0.52x) |        0.002 (  0.14x) |
| figures/f7-34b.pbrt                              |        0.000 |        0.000 (  0.66x) |        0.002 (  0.14x) |
| figures/f7-34c.pbrt                              |        0.000 |        0.000 (  0.57x) |        0.001 (  0.13x) |
| figures/f8-22.pbrt                               |        0.001 |        0.001 (  1.87x) |        0.002 (  0.60x) |
| ganesha/f3-11.pbrt                               |        4.971 |        0.410 ( 12.13x) |        0.406 ( 12.25x) |
| ganesha/ganesha.pbrt                             |        7.702 |        0.408 ( 18.89x) |        0.407 ( 18.95x) |
| hair/curly-hair.pbrt                             |      155.517 |        9.572 ( 16.25x) |       11.993 ( 12.97x) |
| hair/sphere-hairblock.pbrt                       |        0.422 |        0.019 ( 21.91x) |        0.023 ( 18.53x) |
| hair/straight-hair.pbrt                          |       66.121 |        3.974 ( 16.64x) |        4.479 ( 14.76x) |
| head/f9-5.pbrt                                   |        0.072 |        0.025 (  2.89x) |        0.027 (  2.62x) |
| head/head.pbrt                                   |        0.074 |        0.026 (  2.88x) |        0.023 (  3.25x) |
| killeroos/killeroo-gold.pbrt                     |        0.070 |        0.009 (  8.07x) |        0.013 (  5.47x) |
| killeroos/killeroo-moving.pbrt                   |        0.094 |        0.011 (  8.38x) |        0.013 (  7.20x) |
| killeroos/killeroo-simple.pbrt                   |        0.092 |        0.012 (  7.47x) |        0.016 (  5.86x) |
| landscape/f4-1.pbrt                              |       72.900 |        8.605 (  8.47x) |        7.232 ( 10.08x) |
| landscape/f6-13.pbrt                             |       71.291 |        8.381 (  8.51x) |        7.201 (  9.90x) |
| landscape/f6-14.pbrt                             |       70.078 |        8.261 (  8.48x) |        6.971 ( 10.05x) |
| landscape/view-0.pbrt                            |       69.400 |        8.333 (  8.33x) |        6.930 ( 10.02x) |
| landscape/view-1.pbrt                            |       68.819 |        8.239 (  8.35x) |        6.908 (  9.96x) |
| landscape/view-2.pbrt                            |       68.414 |        8.275 (  8.27x) |        6.889 (  9.93x) |
| landscape/view-3.pbrt                            |       67.845 |        8.227 (  8.25x) |        6.867 (  9.88x) |
| landscape/view-4.pbrt                            |       67.749 |        8.231 (  8.23x) |        6.844 (  9.90x) |
| lte-orb/lte-orb-roughglass.pbrt                  |        1.254 |        0.059 ( 21.10x) |        0.062 ( 20.22x) |
| lte-orb/lte-orb-silver.pbrt                      |        1.257 |        0.052 ( 23.98x) |        0.063 ( 20.08x) |
| measure-one/frame120.pbrt                        |       21.605 |       10.616 (  2.04x) |       12.461 (  1.73x) |
| measure-one/frame180.pbrt                        |       20.791 |       11.049 (  1.88x) |       12.165 (  1.71x) |
| measure-one/frame210.pbrt                        |       20.675 |       10.860 (  1.90x) |       12.071 (  1.71x) |
| measure-one/frame25.pbrt                         |       21.890 |       11.027 (  1.99x) |       12.130 (  1.80x) |
| measure-one/frame300.pbrt                        |       20.881 |       11.073 (  1.89x) |       12.021 (  1.74x) |
| measure-one/frame35.pbrt                         |       20.701 |       11.030 (  1.88x) |       11.944 (  1.73x) |
| measure-one/frame380.pbrt                        |       20.310 |       11.011 (  1.84x) |       12.048 (  1.69x) |
| measure-one/frame52.pbrt                         |       20.463 |       11.146 (  1.84x) |       12.065 (  1.70x) |
| measure-one/frame85.pbrt                         |       20.443 |       11.137 (  1.84x) |       12.173 (  1.68x) |
| pbrt-book/book.pbrt                              |        0.097 |        0.024 (  4.06x) |        0.025 (  3.87x) |
| sanmiguel/f10-8.pbrt                             |       25.660 |        4.181 (  6.14x) |        4.302 (  5.96x) |
| sanmiguel/f16-21a.pbrt                           |       25.634 |        4.268 (  6.01x) |        4.279 (  5.99x) |
| sanmiguel/f16-21b.pbrt                           |       25.592 |        4.202 (  6.09x) |        4.302 (  5.95x) |
| sanmiguel/f16-21c.pbrt                           |       25.757 |        4.139 (  6.22x) |        4.287 (  6.01x) |
| sanmiguel/f6-17.pbrt                             |       25.592 |        4.190 (  6.11x) |        4.261 (  6.01x) |
| sanmiguel/f6-25.pbrt                             |       26.666 |        4.731 (  5.64x) |        4.569 (  5.84x) |
| sanmiguel/sanmiguel.pbrt                         |       26.026 |        4.148 (  6.27x) |        4.265 (  6.10x) |
| sanmiguel/sanmiguel_cam1.pbrt                    |       25.586 |        4.105 (  6.23x) |        4.268 (  5.99x) |
| sanmiguel/sanmiguel_cam14.pbrt                   |       19.737 |        4.006 (  4.93x) |        4.126 (  4.78x) |
| sanmiguel/sanmiguel_cam15.pbrt                   |       17.777 |        4.037 (  4.40x) |        4.060 (  4.38x) |
| sanmiguel/sanmiguel_cam18.pbrt                   |       26.108 |        4.281 (  6.10x) |        4.277 (  6.10x) |
| sanmiguel/sanmiguel_cam20.pbrt                   |       26.054 |        4.209 (  6.19x) |        4.292 (  6.07x) |
| sanmiguel/sanmiguel_cam25.pbrt                   |       26.023 |        4.244 (  6.13x) |        4.372 (  5.95x) |
| sanmiguel/sanmiguel_cam3.pbrt                    |       26.001 |        4.327 (  6.01x) |        4.329 (  6.01x) |
| sanmiguel/sanmiguel_cam4.pbrt                    |       25.948 |        4.372 (  5.94x) |        4.367 (  5.94x) |
| simple/anim-bluespheres.pbrt                     |        0.000 |        0.001 (  0.66x) |        0.002 (  0.19x) |
| simple/buddha.pbrt                               |        1.678 |        0.118 ( 14.24x) |        0.109 ( 15.34x) |
| simple/bump-sphere.pbrt                          |        0.001 |        0.001 (  0.80x) |        0.002 (  0.23x) |
| simple/caustic-proj.pbrt                         |        0.000 |        0.001 (  0.42x) |        0.002 (  0.12x) |
| simple/dof-dragons.pbrt                          |       13.063 |        0.330 ( 39.60x) |        0.210 ( 62.35x) |
| simple/miscquads.pbrt                            |        0.001 |        0.001 (  0.89x) |        0.002 (  0.33x) |
| simple/room-mlt.pbrt                             |       failed |        0.014           |        0.017           |
| simple/room-path.pbrt                            |       failed |        0.015           |        0.018           |
| simple/room-sppm.pbrt                            |       failed |        0.014           |        0.016           |
| simple/spheres-differentials-texfilt.pbrt        |        0.000 |        0.001 (  0.59x) |        0.002 (  0.16x) |
| simple/spotfog.pbrt                              |       failed |        0.001           |        0.002           |
| simple/teapot-area-light.pbrt                    |       failed |        0.005           |        0.006           |
| simple/teapot-metal.pbrt                         |        0.101 |        0.005 ( 20.85x) |        0.006 ( 16.10x) |
| smoke-plume/plume-084.pbrt                       |        7.227 |        0.263 ( 27.52x) |        0.271 ( 26.63x) |
| smoke-plume/plume-184.pbrt                       |        8.667 |        0.319 ( 27.20x) |        0.301 ( 28.77x) |
| smoke-plume/plume-284.pbrt                       |        8.720 |        0.315 ( 27.66x) |        0.324 ( 26.89x) |
| sportscar/f12-19a.pbrt                           |        9.486 |        1.466 (  6.47x) |        1.262 (  7.52x) |
| sportscar/f12-19b.pbrt                           |        9.495 |        1.440 (  6.59x) |        1.230 (  7.72x) |
| sportscar/f12-20a.pbrt                           |        9.491 |        1.446 (  6.56x) |        1.251 (  7.58x) |
| sportscar/f12-20b.pbrt                           |        9.380 |        1.419 (  6.61x) |        1.240 (  7.57x) |
| sportscar/f7-37a.pbrt                            |        9.381 |        1.436 (  6.53x) |        1.224 (  7.66x) |
| sportscar/f7-37b.pbrt                            |        9.426 |        1.434 (  6.57x) |        1.229 (  7.67x) |
| sportscar/sportscar.pbrt                         |        9.442 |        1.454 (  6.49x) |        1.222 (  7.73x) |
| sssdragon/dragon_10.pbrt                         |       failed |        0.969           |        0.957           |
| sssdragon/dragon_250.pbrt                        |       failed |        0.966           |        0.955           |
| sssdragon/dragon_50.pbrt                         |       failed |        0.961           |        0.962           |
| sssdragon/f15-7.pbrt                             |       failed |        0.961           |        0.956           |
| structuresynth/arcsphere.pbrt                    |        0.287 |        0.014 ( 20.21x) |        0.016 ( 18.50x) |
| structuresynth/ballpile.pbrt                     |        0.066 |        0.005 ( 12.72x) |        0.007 (  9.75x) |
| structuresynth/metal.pbrt                        |        0.146 |        0.008 ( 19.05x) |        0.009 ( 15.73x) |
| structuresynth/microcity.pbrt                    |        0.538 |        0.026 ( 20.59x) |        0.030 ( 18.04x) |
| transparent-machines/frame1266.pbrt              |        8.057 |        3.509 (  2.30x) |        3.735 (  2.16x) |
| transparent-machines/frame542.pbrt               |        3.082 |        1.297 (  2.38x) |        1.386 (  2.22x) |
| transparent-machines/frame675.pbrt               |        3.890 |        1.710 (  2.27x) |        1.845 (  2.11x) |
| transparent-machines/frame812.pbrt               |        5.068 |        2.262 (  2.24x) |        2.449 (  2.07x) |
| transparent-machines/frame888.pbrt               |        5.951 |        2.646 (  2.25x) |        2.839 (  2.10x) |
| tt/tt.pbrt                                       |        1.632 |        0.740 (  2.21x) |        0.767 (  2.13x) |
| veach-bidir/bidir.pbrt                           |        0.009 |        0.013 (  0.66x) |        0.016 (  0.54x) |
| veach-mis/mis.pbrt                               |        0.001 |        0.006 (  0.16x) |        0.009 (  0.11x) |
| villa/f16-20a.pbrt                               |        5.629 |        1.816 (  3.10x) |        1.906 (  2.95x) |
| villa/f16-20b.pbrt                               |        5.522 |        1.815 (  3.04x) |        1.894 (  2.91x) |
| villa/f16-20c.pbrt                               |        5.580 |        1.810 (  3.08x) |        1.919 (  2.91x) |
| villa/villa-daylight.pbrt                        |        5.425 |        1.828 (  2.97x) |        1.894 (  2.86x) |
| villa/villa-lights-on.pbrt                       |       failed |        1.852           |        1.897           |
| villa/villa-photons.pbrt                         |       failed |        1.816           |        1.891           |
| volume-caustic/caustic.pbrt                      |       failed |        0.001           |        0.002           |
| volume-caustic/f16-22a.pbrt                      |       failed |        0.001           |        0.002           |
| volume-caustic/f16-22b.pbrt                      |       failed |        0.001           |        0.002           |
| vw-van/vw-van.pbrt                               |        3.409 |        0.864 (  3.94x) |        0.899 (  3.79x) |
| white-room/whiteroom-daytime.pbrt                |        1.414 |        0.662 (  2.14x) |        0.720 (  1.96x) |
| white-room/whiteroom-night.pbrt                  |        1.404 |        0.679 (  2.07x) |        0.718 (  1.96x) |
| yeahright/yeahright.pbrt                         |        0.169 |        0.041 (  4.14x) |        0.049 (  3.45x) |


Results for all scenes in Benedikt Bitterli's collection
--------------------------------------------------------

- Source: https://benedikt-bitterli.me/resources/
- The disk cache was prewarmed by running minipbrt and reading (but not
  parsing) any .ply files
- pbrt-parser ran first after the prewarming step, minipbrt ran second,
  threaded ran third.
- Results generated using a release build of pbrt-parsing-perf from commit
  8e6750c4fb5ec2d02964c96d34e6bcfb77d89338.


| Filename                        |  pbrt-parser |     minipbrt (Speedup) |     threaded (Speedup) |
| :------------------------------ | -----------: | ---------------------: | ---------------------: |
| ./bathroom/scene.pbrt           |        0.532 |        0.096 (  5.56x) |        0.031 ( 16.94x) |
| ./bathroom2/scene.pbrt          |        0.975 |        0.091 ( 10.71x) |        0.040 ( 24.63x) |
| ./bedroom/scene.pbrt            |        1.325 |        0.138 (  9.61x) |        0.117 ( 11.35x) |
| ./car/scene.pbrt                |        0.593 |        0.057 ( 10.41x) |        0.031 ( 19.38x) |
| ./car2/scene.pbrt               |        0.812 |        0.077 ( 10.58x) |        0.030 ( 27.46x) |
| ./classroom/scene.pbrt          |        0.095 |        0.011 (  8.96x) |        0.005 ( 18.39x) |
| ./coffee/scene.pbrt             |        0.171 |        0.014 ( 12.52x) |        0.007 ( 23.57x) |
| ./cornell-box/scene.pbrt        |        0.001 |        0.000 (  3.59x) |        0.002 (  0.78x) |
| ./curly-hair/scene.pbrt         |      154.881 |        6.488 ( 23.87x) |        8.178 ( 18.94x) |
| ./dining-room/scene.pbrt        |        0.517 |        0.175 (  2.95x) |        0.217 (  2.39x) |
| ./dragon/scene.pbrt             |        1.392 |        0.179 (  7.79x) |        0.163 (  8.54x) |
| ./furball/scene.pbrt            |      182.831 |       11.545 ( 15.84x) |       14.697 ( 12.44x) |
| ./glass-of-water/scene.pbrt     |        1.486 |        0.318 (  4.67x) |        0.333 (  4.46x) |
| ./hair-curl/scene.pbrt          |      182.791 |       13.354 ( 13.69x) |       16.273 ( 11.23x) |
| ./house/scene.pbrt              |        8.910 |        3.178 (  2.80x) |        3.250 (  2.74x) |
| ./kitchen/scene.pbrt            |        7.207 |        3.006 (  2.40x) |        3.133 (  2.30x) |
| ./lamp/scene.pbrt               |        3.319 |        0.872 (  3.80x) |        0.878 (  3.78x) |
| ./living-room/scene.pbrt        |        0.881 |        0.435 (  2.02x) |        0.479 (  1.84x) |
| ./living-room-2/scene.pbrt      |        3.104 |        1.285 (  2.42x) |        1.384 (  2.24x) |
| ./living-room-3/scene.pbrt      |       49.801 |        2.910 ( 17.11x) |        3.633 ( 13.71x) |
| ./material-testball/scene.pbrt  |        0.663 |        0.139 (  4.77x) |        0.135 (  4.90x) |
| ./spaceship/scene.pbrt          |        2.651 |        0.951 (  2.79x) |        0.976 (  2.72x) |
| ./staircase/scene.pbrt          |        1.454 |        1.779 (  0.82x) |        1.888 (  0.77x) |
| ./staircase2/scene.pbrt         |        0.167 |        0.127 (  1.31x) |        0.145 (  1.15x) |
| ./straight-hair/scene.pbrt      |       87.454 |        6.124 ( 14.28x) |        7.878 ( 11.10x) |
| ./teapot/scene.pbrt             |        0.743 |        0.135 (  5.52x) |        0.141 (  5.26x) |
| ./teapot-full/scene.pbrt        |        0.763 |        0.170 (  4.48x) |        0.175 (  4.35x) |
| ./veach-ajar/scene.pbrt         |        2.241 |        0.441 (  5.08x) |        0.456 (  4.91x) |
| ./veach-bidir/scene.pbrt        |        0.065 |        0.067 (  0.98x) |        0.066 (  0.99x) |
| ./veach-mis/scene.pbrt          |        0.002 |        0.001 (  1.34x) |        0.003 (  0.60x) |
| ./volumetric-caustic/scene.pbrt |        0.001 |        0.001 (  0.77x) |        0.002 (  0.36x) |
| ./water-caustic/scene.pbrt      |        0.553 |        0.087 (  6.39x) |        0.086 (  6.45x) |


Results for Disney's Moana island scene
---------------------------------------

Source: https://www.technology.disneyanimation.com/islandscene
- The disk cache was not prewarmed for these comparisons. The data set is so
  large that whether the data was previously present in the disk cache has 
  very little impact on the overall times.
- pbrt-parser ran first after the prewarming step, minipbrt ran second,
  threaded ran third.

| Filename         |  pbrt-parser |     minipbrt (Speedup) |     threaded (Speedup) |
| :--------------- | -----------: | ---------------------: | ---------------------: |
| pbrt/island.pbrt |     5021.197 |      199.794 ( 25.13x) |      201.739 ( 24.89x) |
