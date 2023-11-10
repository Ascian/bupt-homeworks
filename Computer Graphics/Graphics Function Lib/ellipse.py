import argparse

from PIL import Image
from image_draw import ImageDraw

def parse_args():
    parser = argparse.ArgumentParser(description='Draw ellipse on image')
    
    parser.add_argument(
        "--xs", 
        type=int,
        help="start point x coordinate of the bounding box of the ellipse",
        required=True
    )
    parser.add_argument(
        "--ys", 
        type=int,
        help="start point y coordinate of the bounding box of the ellipse",
        required=True
    )
    parser.add_argument(
        "--xe", 
        type=int,
        help="end point x coordinate of the bounding box of the ellipse",
        required=True
    )
    parser.add_argument(
        "--ye", 
        type=int,
        help="end point y coordinate of the bounding box of the ellipse",
        required=True
    )
    parser.add_argument(
        "--dt", 
        type=float,
        help="granularity of the ellipse, the smaller the dt, the smoother the ellipse",
    )
    parser.add_argument(
        "--width", 
        type=int,
        help="line width",
        default=1
    )
    parser.add_argument(
        "--color", 
        type=str,
        help="color",
        default='black'
    )

    return parser.parse_args()



if __name__ == '__main__':
    args = parse_args()
    image = Image.new('RGB', (100,100), color='white')
    draw = ImageDraw(image)
    draw.ellipse((args.xs, args.ys), (args.xe, args.ye), args.dt, args.width, args.color)
    image.save('ellipse.png')