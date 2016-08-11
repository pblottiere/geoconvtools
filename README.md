# geoconvtools

Various tools for geodata conversion.

## hgt2png

Convert a hgt file (https://dds.cr.usgs.gov/srtm/) to a DEM raster (or
just display it).

For example:

``` bash
> python hgt2png.py N44W073.hgt
```

Then, you may reproject the image thanks to gdal:

```
> gdalwarp -s_srs epsg:4326 -t_srs epsg:2154 hgt_wgs84.png hgt_2154.png
```

And finally to retrieve the bounding box:

```
> gdalinfo hgt_2154.png
```
