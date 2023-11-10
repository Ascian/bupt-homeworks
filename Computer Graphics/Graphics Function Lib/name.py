from PIL import Image
from image_draw import ImageDraw

if __name__ == '__main__':
    # 陈宇
    lines = [((20, 20), (30, 20), 4),
             ((20, 40), (30, 20), 2),  
             ((20, 20), (20, 80), 3),  
             ((20, 35), (27, 45), 2),  
             ((25, 40), (30, 60), 2),  
             ((20, 55), (30, 60), 2),  
             ((40, 30), (80, 30), 4),  
             ((48, 50), (63, 20), 2),
             ((48, 50), (72, 50), 4),
             ((60, 40), (60, 85), 3),
             ((60, 85), (50, 78), 2),
             ((44, 64), (38, 74), 2),
             ((76, 64), (84, 72), 2),
             ((84, 72), (86, 75), 2),
             ((140, 15), (150, 22), 2),
             ((117, 25), (173, 25), 4),
             ((117, 35), (115, 25), 3),
             ((175, 25), (165, 35), 2),
             ((132, 40), (152, 40), 4),
             ((120, 55), (165, 55), 4),
             ((142, 40), (142, 85), 3),
             ((142, 85), (132, 78), 2),
            ]
        
    image = Image.new('RGB', (200,100), color='white')
    draw = ImageDraw(image)
    # draw lines
    for line in lines:
        draw.line(line[0], line[1], line[2])
    image.save('name.png')