Visualizer Plugin for SemRec
============================

This plugin visualizes the live construction of a symbolic task tree from within SemRec. The plugin is in its infancy still, but the following things work already:

 * Connection to SemRec (loadable as a plugin)

 * Initialization and common drawing routines for an SDL2 window

 * Internal task tree representation

 * Capturing of `symbolic-begin-context` and `symbolic-end-context` events from SemRec, and the respective tree transformation

The display of the tree itself is still suboptimal at the moment, but I'm working on that. This should be fixed quite shortly.


Example
-------

![Sample display of SemRec Visualizer](http://robots-doing-things.com/img/visualizer.png)
