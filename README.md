# geoconvtools

Various tools for geodata conversion.

Note that its not fully plug-and-play. Do not be afraid to do some of the dirty
work.

### hgt2png

Convert a hgt file (https://dds.cr.usgs.gov/srtm/) to a DEM raster (or
just display it).

For example:

``` bash
> python hgt2png.py N44W073.hgt
```

Then, you may reproject the image thanks to gdal:

``` bash
> gdalwarp -s_srs epsg:4326 -t_srs epsg:2154 hgt_wgs84.png hgt_2154.png
```

And finally to retrieve the bounding box:

``` bash
> gdalinfo hgt_2154.png
```

### bin2las

Convert N binary files of pointcloud data into 1 LAS file thanks to the PDAL
API. By default, the schema is XYZI where each dimension is encoded as float32
with an offset value.


For example (once compiled) with binary files coming from here https://github.com/iTowns/itowns-sample-data/tree/gh-pages/pointclouds/140616/LR:

``` bash
> bin2las "<BINDIR>/*.bin"  650000.0 6860000.0 0.0 result.las
```

With these particular data, note that Y and Z are inverted and that a factor
is applied to the intensity:

``` bash
Point p;
p.x = x.f + x_offset;
p.y = z.f + y_offset;
p.z = y.f + z_offset;
p.i = i.f*(-10);
```

Then you can check your LAS file with the *lasinfo* tool:

``` bash
> lasinfo result.las
```

### las2pg

Fill a database with pointcloud data coming from a LAS file. The pgwriter from
PDAL is used.

Firt edit *las2pg.sh* to update the database name and then edit *pipe.jon* in
order to update the database and the name of the las file. Then:

``` bash
> sh las2pg.sh
```
