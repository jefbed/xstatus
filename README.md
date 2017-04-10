# xstatus
simple X11 status bar and launcher for lightweight window managers

# Custom Status Text
The ruby utility rbstatus.rb is included as an example of how to provide
a custom updating status string to xstatus.  xstatus reads /tmp/xstatus
periodically, displaying the text therein.  You may have to modify the
files polled in rbstatus.rb to reflect your own system.  

# TO-DO
* Handle widget failure gracefully--If something is not available,
simply disable it dynamically.
* Compute geometries, rather than using so many fixed values.
* Add menus
* Add Xft support

