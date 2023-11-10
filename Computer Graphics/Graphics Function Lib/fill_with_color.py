import argparse

from PIL import Image
from image_draw import ImageDraw

def parse_args():
    parser = argparse.ArgumentParser(description='Fill polygon with color on image')
    
    parser.add_argument(
        "--xy", 
        nargs='+',
        type=int,
        help="coordinates of the polygon, list as x1 y1 x2 y2 x3 y3 ...",
        required=True
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
    draw.fill_with_color(args.xy, args.color)
    image.save('fill_with_color.png')