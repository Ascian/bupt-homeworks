import argparse

from PIL import Image
from image_draw import ImageDraw

def parse_args():
    parser = argparse.ArgumentParser(description='Draw line on image')
    
    parser.add_argument(
        "--xs", 
        type=int,
        help="start point x coordinate",
        required=True
    )
    parser.add_argument(
        "--ys", 
        type=int,
        help="start point y coordinate",
        required=True
    )
    parser.add_argument(
        "--xe", 
        type=int,
        help="end point x coordinate",
        required=True
    )
    parser.add_argument(
        "--ye", 
        type=int,
        help="end point y coordinate",
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
    parser.add_argument(
        "--type", 
        type=str,
        help="line type",
        default='solid',
        choices=['solid', 'dotted', 'dashed', 'dashed-dotted']
    )

    return parser.parse_args()



if __name__ == '__main__':
    args = parse_args()
    image = Image.new('RGB', (100,100), color='white')
    draw = ImageDraw(image)
    draw.line((args.xs, args.ys), (args.xe, args.ye), args.width, args.color, args.type)
    image.save('line.png')