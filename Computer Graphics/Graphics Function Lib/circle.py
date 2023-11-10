import argparse

from PIL import Image
from image_draw import ImageDraw

def parse_args():
    parser = argparse.ArgumentParser(description='Draw circle on image')
    
    parser.add_argument(
        "--x", 
        type=int,
        help="x of the circle center",
        required=True
    )
    parser.add_argument(
        "--y", 
        type=int,
        help="y of the circle center",
        required=True
    )
    parser.add_argument(
        "--r", 
        type=int,
        help="radius",
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
    draw.circle((args.x, args.y), args.r, args.width, args.color)
    image.save('circle.png')