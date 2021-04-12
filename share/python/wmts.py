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

def array_to_raster(array):
    """Array > Raster
    Save a raster from a C order array.

    :param array: ndarray
    """
    dst_filename = '/a_file/name.tiff'


    # You need to get those values like you did.
    x_pixels = 16  # number of pixels in x
    y_pixels = 16  # number of pixels in y
    PIXEL_SIZE = 3  # size of the pixel...        
    x_min = 553648  
    y_max = 7784555  # x_min & y_max are like the "top left" corner.
    wkt_projection = 'a projection in wkt that you got from other file'

    driver = gdal.GetDriverByName('GTiff')

    dataset = driver.Create(
        dst_filename,
        x_pixels,
        y_pixels,
        1,
        gdal.GDT_Float32, )

    dataset.SetGeoTransform((
        x_min,    # 0
        PIXEL_SIZE,  # 1
        0,                      # 2
        y_max,    # 3
        0,                      # 4
        -PIXEL_SIZE))  

    dataset.SetProjection(wkt_projection)
    dataset.GetRasterBand(1).WriteArray(array)
    dataset.FlushCache()  # Write to disk.
    return dataset, dataset.GetRasterBand(1)  #If you need to return, remenber to return  also the dataset because the band don`t live without dataset.

#https://stackoverflow.com/questions/60282305/how-i-can-read-corner-coordinates-in-degrees
def degreesToMeters( ulx, uly, lrx, lry ):
#def main():
    #wkt_srs = ds.GetProjection()
    wkt_srs = cartopy.crs.PlateCarree()
    gt = ds.GetGeoTransform()
    xs = ds.RasterXSize
    ys = ds.RasterYSize

    ulx, uly = gdal.ApplyGeoTransform(gt, 0, 0)
    lrx, lry = gdal.ApplyGeoTransform(gt, xs, ys)

    tar_srs = gdal.osr.SpatialReference()
    tar_srs.ImportFromWkt(wkt_srs)

    src_srs = gdal.osr.SpatialReference()
    src_srs.ImportFromEPSG(4326)

    # with recent versions of GDAL the axis order (x,y vs y,x) depends
    # on the projection. Force "x,y" with:
    src_srs.SetAxisMappingStrategy(gdal.osr.OAMS_TRADITIONAL_GIS_ORDER)
    tar_srs.SetAxisMappingStrategy(gdal.osr.OAMS_TRADITIONAL_GIS_ORDER)

    ct = gdal.osr.CoordinateTransformation(src_srs, tar_srs)

    ulx_deg, uly_deg = ct.TransformPoint(ulx, uly)
    lrx_deg, lry_deg = ct.TransformPoint(lrx, lry)

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
    #            options=warpOpts )
     
    '''ewPixelRes = ( east  - west  ) / width
    snPixelRes = ( south - north ) / height
    print( str(ewPixelRes) + " " + str(snPixelRes) )
    tiff.SetGeoTransform( [ west, ewPixelRes, 0, north, -snPixelRes, 0 ] )'''
    
    '''dataset.SetGeoTransform((
        x_min,    # 0
        PIXEL_SIZE,  # 1
        0,                      # 2
        y_max,    # 3
        0,                      # 4
        -PIXEL_SIZE))  '''
    #tiff.SetProjection( platteCarree )

if __name__ == '__main__':
    main()
