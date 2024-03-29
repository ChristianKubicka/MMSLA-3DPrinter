#!/usr/bin/env python3
# imports
import math
import trimesh
import sys
import os

# Resolution Variables
horizontal_resolution = 1600
vertical_resolution = 1000

# Physical Screen Variables
screen_length_in = 6.23
screen_aspect_ratio = 1.6
screen_length_mm = 6.23 * 25.4
screen_height_mm = math.sqrt((screen_length_mm * screen_length_mm) / (1 + (screen_aspect_ratio * screen_aspect_ratio)))
screen_width_mm = screen_height_mm * screen_aspect_ratio
pixel_pitch = screen_width_mm / horizontal_resolution
layer_width_mm = 0.070
origin_x = -1 * screen_width_mm / 2
origin_y = -1 * screen_height_mm / 2


def main():
    # Getting command-line arguments
    # args = parse_arguments()

    # Loading a mesh and getting the units
    mesh = trimesh.load_mesh("3dbenchy.stl")
    mesh, units = as_mesh_with_units(mesh)

    # Slicing the model
    layer_count = slice_mesh(mesh, "TestPrint")
    print("Layer Count: " + str(layer_count))


def parse_arguments():
    # Checking for the correct number of input arguments
    if len(sys.argv) != 3:
        print("Usage: ./slicer <filename> <output>")
        exit(1)

    # Attempting to make output directory
    path = os.path.join("./", sys.argv[2])
    try:
        os.makedirs(path)
    except:
        print("Directory Already Exists")
        exit(1)

    # Returning Filename and Output
    return sys.argv[1], sys.argv[2]


def slice_mesh(mesh, output_filename):
    # Creating Layer Variable (Each Layer is 100 microns)
    layer = 0

    # Slicing until a cross-section does not exist
    while True:

        # Breaking if the slice does not exist
        mesh_slice = mesh.section(plane_normal=[0, 0, 1], plane_origin=[0, 0, layer * layer_width_mm])

        if mesh_slice is None:
            break

        # Converting the slice to a PNG with the appropriate file name
        get_png_of_layer(mesh_slice, output_filename + "/IMG_" + str(layer) + ".png")

        # Incrementing the layer
        layer = layer + 1

    # Returning the number of layers
    return layer - 1


def get_png_of_layer(slice, png_name):
    # From: https://github.com/mikedh/trimesh/issues/1359
    # It is unknown why this must be done
    to_2D = trimesh.geometry.align_vectors([0, 0, 1], [0, 0, 1])

    # Getting the planar representation of the slice
    slice_2D, to_3D = slice.to_planar(to_2D=to_2D)

    # Rasterizing the image
    img = slice_2D.rasterize(pitch=[pixel_pitch, pixel_pitch],
                             origin=[origin_x, origin_y],
                             resolution=[horizontal_resolution, vertical_resolution],
                             fill=True,
                             width=0
                             )

    # Saving the image
    img.save(png_name)


# From: https://github.com/mikedh/trimesh/issues/504
def as_mesh_with_units(scene_or_mesh):
    if isinstance(scene_or_mesh, trimesh.Scene):
        units = scene_or_mesh.units
        mesh = trimesh.util.concatenate([
            trimesh.Trimesh(vertices=m.vertices, faces=m.faces)
            for m in scene_or_mesh.geometry.values()])
    else:
        mesh = scene_or_mesh
        units = 'millimeter'
    return mesh, units


main()
