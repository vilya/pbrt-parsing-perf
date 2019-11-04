Results from pbrt-parsing-perf
==============================

Notes
-----

pbrt-parser loads PLY meshes but minipbrt doesn't (yet), so it's doing a lot
more work on the scenes which use PLY meshes.

pbrt-parser only supports a (useful common) subset of the PBRT format, whereas
minipbrt supports the entire format. The subset supported by pbrt-parser is:
- Shapes: trianglemesh, curve, sphere and disk only (plus its own non-standard
  quadmesh type).
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

The interesting results below are the scenes with no PLY files. They're the
only ones which currently give a fair comparison.

Of the scenes with no PLY files, minipbrt is an order of magnitude faster than
pbrt-parser (between 19 and 42 times faster). The largest speedups are on the
hair files, which contain lots of inline curve primitives; and the Moana
island scene which has a lot of trianglemesh primitives.

There are some scenes where minipbrt appears to be slower than pbrt-parser but
this seems purely due to minipbrt running first, which effectively pre-warms
the disk cache for the pbrt-parser run. If we reverse their order then
minipbrt reports much faster results than pbrt-parser for those scenes (as
well as all the others).



Results for Disney's Moana island scene
---------------------------------------

Filename                     minipbrt   pbrt-parser       speedup
-----------------------  ------------  ------------  ------------
island/pbrt/island.pbrt       163.517      5387.676        32.95x



Results for scenes in the pbrt-v3-scenes collection with no PLY files, with minipbrt running first
--------------------------------------------------------------------------------------------------

Filename                             minipbrt   pbrt-parser       speedup
-------------------------------  ------------  ------------  ------------
figures/f10-1ac.pbrt                    0.001         0.001         0.86x
figures/f10-1b.pbrt                     0.000         0.000         0.95x
figures/f11-15.pbrt                     0.000         0.000         1.13x
figures/f3-18.pbrt                      0.000        failed             -
figures/f7-19a.pbrt                     0.000         0.000         0.86x
figures/f7-19b.pbrt                     0.000         0.000         0.73x
figures/f7-19c.pbrt                     0.000         0.000         0.68x
figures/f7-30a.pbrt                     0.000         0.000         0.67x
figures/f7-30b.pbrt                     0.000         0.000         0.66x
figures/f7-30c.pbrt                     0.000         0.000         0.68x
figures/f7-34a.pbrt                     0.000         0.000         0.68x
figures/f7-34b.pbrt                     0.000         0.000         0.63x
figures/f7-34c.pbrt                     0.000         0.000         0.64x
figures/f8-22.pbrt                      0.001         0.002         2.76x
hair/curly-hair.pbrt                    4.392       154.178        35.11x
hair/sphere-hairblock.pbrt              0.012         0.333        27.61x
hair/straight-hair.pbrt                 1.722        54.531        31.67x
killeroos/killeroo-gold.pbrt            0.002         0.062        28.07x
killeroos/killeroo-moving.pbrt          0.003         0.084        31.08x
killeroos/killeroo-simple.pbrt          0.002         0.101        42.05x
lte-orb/lte-orb-roughglass.pbrt         0.045         1.206        27.08x
lte-orb/lte-orb-silver.pbrt             0.035         1.202        34.79x
smoke-plume/plume-084.pbrt              0.270         7.375        27.33x
smoke-plume/plume-184.pbrt              0.308         8.802        28.60x
smoke-plume/plume-284.pbrt              0.314         8.879        28.30x
structuresynth/arcsphere.pbrt           0.012         0.278        23.45x
structuresynth/ballpile.pbrt            0.004         0.068        19.02x
structuresynth/metal.pbrt               0.006         0.135        23.51x
structuresynth/microcity.pbrt           0.020         0.536        26.53x
volume-caustic/caustic.pbrt             0.000        failed             -
volume-caustic/f16-22a.pbrt             0.000        failed             -
volume-caustic/f16-22b.pbrt             0.000        failed             -



Results for scenes in the pbrt-v3-scenes collection with no PLY files, with pbrt-parser running first
-----------------------------------------------------------------------------------------------------

Filename                             minipbrt   pbrt-parser       speedup
-------------------------------  ------------  ------------  ------------
figures/f10-1ac.pbrt                    0.000         0.006        14.75x
figures/f10-1b.pbrt                     0.000         0.005        10.85x
figures/f11-15.pbrt                     0.000         0.005        14.64x
figures/f3-18.pbrt                      0.000        failed             -
figures/f7-19a.pbrt                     0.000         0.006        16.64x
figures/f7-19b.pbrt                     0.000         0.005        15.42x
figures/f7-19c.pbrt                     0.000         0.005        13.06x
figures/f7-30a.pbrt                     0.000         0.007        19.54x
figures/f7-30b.pbrt                     0.000         0.004        13.22x
figures/f7-30c.pbrt                     0.000         0.004        13.50x
figures/f7-34a.pbrt                     0.000         0.005        14.58x
figures/f7-34b.pbrt                     0.000         0.005        13.89x
figures/f7-34c.pbrt                     0.000         0.005        11.65x
figures/f8-22.pbrt                      0.001         0.019        22.47x
hair/curly-hair.pbrt                    4.930       142.887        28.98x
hair/sphere-hairblock.pbrt              0.013         0.487        36.90x
hair/straight-hair.pbrt                 1.734        54.008        31.15x
killeroos/killeroo-gold.pbrt            0.002         0.103        47.45x
killeroos/killeroo-moving.pbrt          0.003         0.134        48.08x
killeroos/killeroo-simple.pbrt          0.003         0.093        32.67x
lte-orb/lte-orb-roughglass.pbrt         0.040         1.328        33.27x
lte-orb/lte-orb-silver.pbrt             0.041         1.375        33.66x
smoke-plume/plume-084.pbrt              0.281         8.042        28.62x
smoke-plume/plume-184.pbrt              0.333         9.478        28.44x
smoke-plume/plume-284.pbrt              0.336         9.578        28.53x
structuresynth/arcsphere.pbrt           0.011         0.307        27.27x
structuresynth/ballpile.pbrt            0.004         0.081        21.48x
structuresynth/metal.pbrt               0.006         0.166        26.21x
structuresynth/microcity.pbrt           0.020         0.584        29.28x
volume-caustic/caustic.pbrt             0.001        failed             -
volume-caustic/f16-22a.pbrt             0.000        failed             -
volume-caustic/f16-22b.pbrt             0.001        failed             -



Results for ALL scenes in the pbrt-v3-scenes collection
-------------------------------------------------------

Filename                                              minipbrt   pbrt-parser
------------------------------------------------  ------------  ------------
barcelona-pavilion/pavilion-day.pbrt                      0.00          5.33
barcelona-pavilion/pavilion-night.pbrt                    0.00          5.27
bathroom/bathroom.pbrt                                    0.00          0.33
bmw-m6/bmw-m6.pbrt                                        0.00          0.63
breakfast/breakfast.pbrt                                  0.00        failed
breakfast/breakfast-lamps.pbrt                            0.01          0.61
breakfast/f16-8a.pbrt                                     0.01          0.36
breakfast/f16-8b.pbrt                                     0.01          0.36
buddha-fractal/buddha-fractal.pbrt                        0.04          0.69
bunny-fur/f3-15.pbrt                                      2.09         57.60
caustic-glass/f16-11a.pbrt                                0.00        failed
caustic-glass/f16-11b.pbrt                                0.00        failed
caustic-glass/f16-9a.pbrt                                 0.00        failed
caustic-glass/f16-9b.pbrt                                 0.00        failed
caustic-glass/f16-9c.pbrt                                 0.00        failed
caustic-glass/glass.pbrt                                  0.00        failed
chopper-titan/chopper-titan.pbrt                          0.01          5.20
cloud/cloud.pbrt                                          0.02          0.35
cloud/f15-4a.pbrt                                         0.02          0.34
cloud/f15-4b.pbrt                                         0.02          0.35
cloud/f15-4c.pbrt                                         0.02          0.35
cloud/smoke.pbrt                                          0.02          0.35
coffee-splash/f15-5.pbrt                                  0.02          0.28
coffee-splash/splash.pbrt                                 0.00          0.23
contemporary-bathroom/contemporary-bathroom.pbrt          0.02          5.80
crown/crown.pbrt                                          0.01          6.76
dambreak/dambreak0.pbrt                                   0.01          1.06
dambreak/dambreak1.pbrt                                   0.01          1.66
dragon/f11-13.pbrt                                        0.01          2.99
dragon/f11-14.pbrt                                        0.01          2.89
dragon/f14-3.pbrt                                         0.01          1.44
dragon/f14-5.pbrt                                         0.01          2.87
dragon/f15-13.pbrt                                        0.00          3.15
dragon/f8-10.pbrt                                         0.01          1.55
dragon/f8-14a.pbrt                                        0.01          1.54
dragon/f8-14b.pbrt                                        0.00          1.47
dragon/f8-21a.pbrt                                        0.00          1.57
dragon/f8-21b.pbrt                                        0.01          1.60
dragon/f8-24.pbrt                                         0.01          2.98
dragon/f8-4a.pbrt                                         0.01          1.49
dragon/f8-4b.pbrt                                         0.00          1.49
dragon/f9-3.pbrt                                          0.00          1.51
dragon/f9-4.pbrt                                          0.01          1.61
ecosys/ecosys.pbrt                                        0.02          2.82
figures/f10-1ac.pbrt                                      0.00          0.00
figures/f10-1b.pbrt                                       0.01          0.00
figures/f11-15.pbrt                                       0.01          0.00
figures/f3-18.pbrt                                        0.00        failed
figures/f7-19a.pbrt                                       0.01          0.00
figures/f7-19b.pbrt                                       0.00          0.00
figures/f7-19c.pbrt                                       0.00          0.00
figures/f7-30a.pbrt                                       0.00          0.00
figures/f7-30b.pbrt                                       0.00          0.00
figures/f7-30c.pbrt                                       0.00          0.00
figures/f7-34a.pbrt                                       0.01          0.00
figures/f7-34b.pbrt                                       0.00          0.00
figures/f7-34c.pbrt                                       0.00          0.00
figures/f8-22.pbrt                                        0.01          0.00
ganesha/f3-11.pbrt                                        0.00          4.14
ganesha/ganesha.pbrt                                      0.01          3.83
hair/curly-hair.pbrt                                      4.61        167.63
hair/sphere-hairblock.pbrt                                0.03          0.32
hair/straight-hair.pbrt                                   1.71         55.04
head/f9-5.pbrt                                            0.01          0.03
head/head.pbrt                                            0.01          0.02
killeroos/killeroo-gold.pbrt                              0.06          0.05
killeroos/killeroo-moving.pbrt                            0.05          0.08
killeroos/killeroo-simple.pbrt                            0.01          0.08
landscape/f4-1.pbrt                                       0.07         36.14
landscape/f6-13.pbrt                                      0.06         32.22
landscape/f6-14.pbrt                                      0.05         32.02
landscape/view-0.pbrt                                     0.05         36.37
landscape/view-1.pbrt                                     0.09         49.21
landscape/view-2.pbrt                                     0.09         38.35
landscape/view-3.pbrt                                     0.06         32.55
landscape/view-4.pbrt                                     0.06         31.83
lte-orb/lte-orb-roughglass.pbrt                           0.06          1.28
lte-orb/lte-orb-silver.pbrt                               0.05          1.29
measure-one/frame120.pbrt                                 0.05         15.28
measure-one/frame180.pbrt                                 0.03          8.29
measure-one/frame210.pbrt                                 0.03          8.01
measure-one/frame25.pbrt                                  0.03          8.29
measure-one/frame300.pbrt                                 0.03          8.07
measure-one/frame35.pbrt                                  0.03          8.01
measure-one/frame380.pbrt                                 0.03          7.95
measure-one/frame52.pbrt                                  0.04          8.15
measure-one/frame85.pbrt                                  0.04          8.04
pbrt-book/book.pbrt                                       0.00          0.06
sanmiguel/f10-8.pbrt                                      0.41         23.33
sanmiguel/f16-21a.pbrt                                    0.21         16.93
sanmiguel/f16-21b.pbrt                                    0.24         17.10
sanmiguel/f16-21c.pbrt                                    0.21         17.22
sanmiguel/f6-17.pbrt                                      0.22         17.07
sanmiguel/f6-25.pbrt                                      0.23         17.11
sanmiguel/sanmiguel.pbrt                                  0.22         17.12
sanmiguel/sanmiguel_cam1.pbrt                             0.22         16.86
sanmiguel/sanmiguel_cam14.pbrt                            0.10         11.05
sanmiguel/sanmiguel_cam15.pbrt                            0.07          9.05
sanmiguel/sanmiguel_cam18.pbrt                            0.23         16.99
sanmiguel/sanmiguel_cam20.pbrt                            0.20         16.88
sanmiguel/sanmiguel_cam25.pbrt                            0.22         16.76
sanmiguel/sanmiguel_cam3.pbrt                             0.20         16.66
sanmiguel/sanmiguel_cam4.pbrt                             0.22         17.09
simple/anim-bluespheres.pbrt                              0.01          0.00
simple/buddha.pbrt                                        0.00          0.98
simple/bump-sphere.pbrt                                   0.01          0.00
simple/caustic-proj.pbrt                                  0.00          0.00
simple/dof-dragons.pbrt                                   0.01         12.86
simple/miscquads.pbrt                                     0.01          0.00
simple/room-mlt.pbrt                                      0.03        failed
simple/room-path.pbrt                                     0.01        failed
simple/room-sppm.pbrt                                     0.01        failed
simple/spheres-differentials-texfilt.pbrt                 0.01          0.00
simple/spotfog.pbrt                                       0.00        failed
simple/teapot-area-light.pbrt                             0.01        failed
simple/teapot-metal.pbrt                                  0.02          0.10
smoke-plume/plume-084.pbrt                                0.28          7.49
smoke-plume/plume-184.pbrt                                0.32          8.82
smoke-plume/plume-284.pbrt                                0.32          9.48
sportscar/f12-19a.pbrt                                    0.02          6.68
sportscar/f12-19b.pbrt                                    0.01          6.33
sportscar/f12-20a.pbrt                                    0.01          6.18
sportscar/f12-20b.pbrt                                    0.01          6.32
sportscar/f7-37a.pbrt                                     0.01          6.32
sportscar/f7-37b.pbrt                                     0.01          6.28
sportscar/sportscar.pbrt                                  0.01          6.10
sssdragon/dragon_10.pbrt                                  0.01        failed
sssdragon/dragon_250.pbrt                                 0.00        failed
sssdragon/dragon_50.pbrt                                  0.00        failed
sssdragon/f15-7.pbrt                                      0.00        failed
structuresynth/arcsphere.pbrt                             0.02          0.28
structuresynth/ballpile.pbrt                              0.02          0.07
structuresynth/metal.pbrt                                 0.02          0.14
structuresynth/microcity.pbrt                             0.03          0.53
transparent-machines/frame1266.pbrt                       0.01          8.17
transparent-machines/frame542.pbrt                        0.01          3.04
transparent-machines/frame675.pbrt                        0.01          3.91
transparent-machines/frame812.pbrt                        0.01          5.14
transparent-machines/frame888.pbrt                        0.01          6.18
tt/tt.pbrt                                                0.01          2.53
veach-bidir/bidir.pbrt                                    0.00          0.08
veach-mis/mis.pbrt                                        0.00          0.02
villa/f16-20a.pbrt                                        0.04          4.40
villa/f16-20b.pbrt                                        0.01          2.61
villa/f16-20c.pbrt                                        0.01          2.66
villa/villa-daylight.pbrt                                 0.01          2.57
villa/villa-lights-on.pbrt                                0.01        failed
villa/villa-photons.pbrt                                  0.01        failed
volume-caustic/caustic.pbrt                               0.01        failed
volume-caustic/f16-22a.pbrt                               0.01        failed
volume-caustic/f16-22b.pbrt                               0.00        failed
vw-van/vw-van.pbrt                                        0.01          2.42
white-room/whiteroom-daytime.pbrt                         0.02          1.63
white-room/whiteroom-night.pbrt                           0.01          0.59
yeahright/yeahright.pbrt                                  0.01          0.15



Results for ALL scenes in Benedikt Bitterli's collection with minipbrt running first
------------------------------------------------------------------------------------

Filename                             minipbrt   pbrt-parser       speedup
-------------------------------  ------------  ------------  ------------
./bathroom/scene.pbrt                   0.007         5.001       679.94x
./bathroom2/scene.pbrt                  0.005         1.436       268.21x
./bedroom/scene.pbrt                    0.005         1.970       371.82x
./car/scene.pbrt                        0.006         0.862       149.01x
./car2/scene.pbrt                       0.005         1.335       255.49x
./classroom/scene.pbrt                  0.005         0.446        82.42x
./coffee/scene.pbrt                     0.005         0.276        58.75x
./cornell-box/scene.pbrt                0.006         0.001         0.23x
./curly-hair/scene.pbrt                 4.521       153.612        33.98x
./dining-room/scene.pbrt                0.006         0.546        91.32x
./dragon/scene.pbrt                     0.004         0.863       201.95x
./furball/scene.pbrt                    5.408       168.579        31.17x
./glass-of-water/scene.pbrt             0.007         0.572        87.54x
./hair-curl/scene.pbrt                  5.227       145.522        27.84x
./house/scene.pbrt                      0.008         4.191       558.50x
./kitchen/scene.pbrt                    0.006         3.778       633.38x
./lamp/scene.pbrt                       0.005         1.397       262.37x
./living-room/scene.pbrt                0.006         0.724       122.56x
./living-room-2/scene.pbrt              0.006         2.036       369.28x
./living-room-3/scene.pbrt              0.813        30.754        37.84x
./material-testball/scene.pbrt          0.005         0.284        52.05x
./spaceship/scene.pbrt                  0.008         1.496       188.16x
./staircase/scene.pbrt                  0.006         4.446       716.43x
./staircase2/scene.pbrt                 0.005         0.171        35.31x
./straight-hair/scene.pbrt              2.678        64.583        24.12x
./teapot/scene.pbrt                     0.006         0.195        32.72x
./teapot-full/scene.pbrt                0.005         0.204        41.88x
./veach-ajar/scene.pbrt                 0.005         0.823       153.29x
./veach-bidir/scene.pbrt                0.005         0.081        15.39x
./veach-mis/scene.pbrt                  0.006         0.002         0.30x
./volumetric-caustic/scene.pbrt         0.005         0.001         0.17x
./water-caustic/scene.pbrt              0.005         0.149        29.14x



Results for ALL scenes in Benedikt Bitterli's collection with pbrt-parser running first
---------------------------------------------------------------------------------------

Filename                             minipbrt   pbrt-parser       speedup
-------------------------------  ------------  ------------  ------------
./bathroom/scene.pbrt                   0.001         5.766      4455.25x
./bathroom2/scene.pbrt                  0.001         1.620      2715.94x
./bedroom/scene.pbrt                    0.001         2.153      3487.84x
./car/scene.pbrt                        0.000         0.962      2054.32x
./car2/scene.pbrt                       0.000         1.491      3028.83x
./classroom/scene.pbrt                  0.001         0.521      1012.43x
./coffee/scene.pbrt                     0.001         0.321       594.65x
./cornell-box/scene.pbrt                0.000         0.008        18.23x
./curly-hair/scene.pbrt                 4.972       148.207        29.81x
./dining-room/scene.pbrt                0.000         0.597      1205.91x
./dragon/scene.pbrt                     0.000         0.931      2071.56x
./furball/scene.pbrt                    7.371       165.983        22.52x
./glass-of-water/scene.pbrt             0.001         0.921      1736.66x
./hair-curl/scene.pbrt                  6.347       149.866        23.61x
./house/scene.pbrt                      0.001         5.646      5326.74x
./kitchen/scene.pbrt                    0.001         4.950      5650.69x
./lamp/scene.pbrt                       0.001         1.863      2155.55x
./living-room/scene.pbrt                0.001         0.830      1256.99x
./living-room-2/scene.pbrt              0.001         2.510      2712.67x
./living-room-3/scene.pbrt              0.912        41.558        45.55x
./material-testball/scene.pbrt          0.001         0.342       530.38x
./spaceship/scene.pbrt                  0.001         1.753      2024.53x
./staircase/scene.pbrt                  0.001         4.998      3918.38x
./staircase2/scene.pbrt                 0.001         0.215       294.57x
./straight-hair/scene.pbrt              2.963        67.439        22.76x
./teapot/scene.pbrt                     0.001         0.316       595.28x
./teapot-full/scene.pbrt                0.001         0.325       586.24x
./veach-ajar/scene.pbrt                 0.001         1.201      1952.40x
./veach-bidir/scene.pbrt                0.001         0.102       195.60x
./veach-mis/scene.pbrt                  0.001         0.007         9.65x
./volumetric-caustic/scene.pbrt         0.001         0.006         9.31x
./water-caustic/scene.pbrt              0.001         0.260       446.17x
