import scrapy


class RentItem(scrapy.Item):
    id = scrapy.Field()  # 房屋的 data-house_code，用于查重处理
    type = scrapy.Field()  # 房屋类型，包括整租、合租、独栋
    region = scrapy.Field()  # 所在区
    plate = scrapy.Field()  # 所在板块
    location = scrapy.Field()  # 具体位置
    area = scrapy.Field()  # 面积
    facing = scrapy.Field()  # 朝向
    room_num = scrapy.Field()  # 室数量
    hall_num = scrapy.Field()  # 厅数量
    toilet_num = scrapy.Field()  # 卫生间数量
    price_info = scrapy.Field()  # 租金价格，单位元/月
