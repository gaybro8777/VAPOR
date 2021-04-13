"""
Interactive WMTS (Web Map Tile Service)
---------------------------------------

This example demonstrates the interactive pan and zoom capability
supported by an OGC web services Web Map Tile Service (WMTS) aware axes.

The example WMTS layer is a single composite of data sampled over nine days
in April 2012 and thirteen days in October 2012 showing the Earth at night.
It does not vary over time.

The imagery was collected by the Suomi National Polar-orbiting Partnership
(Suomi NPP) weather satellite operated by the United States National Oceanic
and Atmospheric Administration (NOAA).

"""
import matplotlib.gridspec as gridspec
import matplotlib.pyplot as plt
import cartopy.crs as ccrs
import PIL
from osgeo import gdal

def main():
    url = 'https://map1c.vis.earthdata.nasa.gov/wmts-geo/wmts.cgi'
    layer = 'VIIRS_CityLights_2012'
    #layer = 'GOES-West_ABI_Band2_Red_Visible_1km'

    west = -170 
    north = 60 
    east = -10 
    south = 15   

    width = 1920
    height = 1080

    dpi = 100 
    fig = plt.figure( figsize=(width/dpi, height/dpi), tight_layout=True )
    ax = fig.add_subplot(1, 1, 1, projection=ccrs.PlateCarree())
    ax.add_wmts(url, layer)
    ax.set_extent([west, east, south, north], crs=ccrs.PlateCarree())
    ax.coastlines(resolution='50m', color='yellow')

    tiffFile = "/Users/pearse/night6.tif"
    fig.savefig( tiffFile,
                 bbox_inches='tight',
                 pad_inches=0 )

    platteCarree = "+proj=eqc +lat_ts=0 +lat_0=0 +lon_0=0 +x_0=0 +y_0=0 +ellps=WGS84"

    tiff    = gdal.OpenShared( tiffFile, gdal.GA_Update)
    width   = tiff.RasterXSize
    height  = tiff.RasterYSize
  
    nightTranslated = '/Users/pearse/nightTranslated.tif'
    nightWarped     = '/Users/pearse/nightWarped.tif'

    gdal.Translate( srcDS=tiffFile, 
                    destName=nightTranslated,
                    format = 'GTiff', 
                    outputBounds = [ west, north, east, south ],
                    outputSRS = 'EPSG:4326'
    )

    warpOpts = gdal.WarpOptions(
        srcSRS='EPSG:4326',
        dstSRS='EPSG:32662'
    )
    gdal.Warp(  destNameOrDestDS=nightWarped, 
                srcDSOrSrcDSTab=nightTranslated, 
                srcSRS = 'EPSG:4326',
                dstSRS='EPSG:32662' )

if __name__ == '__main__':
    main()
