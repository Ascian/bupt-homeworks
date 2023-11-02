import re

import scrapy

from lianjia_data_process.items import RentItem


# 租房数据爬取基类
class Spider(scrapy.spiders.Spider):
    start_area = 1  # 爬取面积下限
    end_area = 10000  # 爬取面积上限
    start_price = 1  # 爬取租金下限
    end_price = 1000000  # 爬取租金上限

    def __init__(self):
        self.regions = None
        self.log_file = open('..\log.txt', 'w', encoding='utf-8')

    # 设置面积约束缩小查询范围
    def parse(self, response):
        # 判断查询结果是否为 0
        if response.xpath('/html/body/div[3]/div[1]/div[5]/div[1]/div[1]/@class').extract()[0] == 'content__list':
            total_count = int(
                response.xpath('/html/body/div[3]/div[1]/div[5]/div[1]/p/span[1]/text()').extract()[0])  # 房屋总数
            _ = re.split(r'(rco31rba\d+rea\d+)', response.url)
            if total_count > 3000:
                # 房屋总数超过3000，缩小查询范围
                rba, rea = map(int, re.search(r'rba(\d+)rea(\d+)', response.url).group(1, 2))
                if rea - rba == 1:
                    # 面积约束已达到最小，使用租金价格约束继续缩小查询范围
                    self.log_file.write('筛选大于3000: ' + response.url + '\n')
                    yield scrapy.Request(url=_[0] + _[1] + f'brp{Spider.start_price}erp{Spider.end_price}',
                                         callback=self.parse_price, dont_filter=True)
                else:
                    # 二分法缩小查询范围
                    mid = int((rba + rea) / 2)
                    yield scrapy.Request(url=_[0] + f'rco31rba{rba}rea{mid}',
                                         callback=self.parse, dont_filter=True)
                    yield scrapy.Request(url=_[0] + f'rco31rba{mid}rea{rea}',
                                         callback=self.parse, dont_filter=True)
            else:
                # 房屋数量少于3000，爬取每一页的数据
                for i in range(1, int((total_count - 1) / 30) + 2):
                    yield scrapy.Request(url=_[0] + f'pg{i}' + _[1], callback=self.parse_item, dont_filter=True)

    # 设置租金约束缩小查询范围
    def parse_price(self, response):
        # 判断查询结果是否为 0
        if response.xpath('/html/body/div[3]/div[1]/div[5]/div[1]/div[1]/@class').extract()[0] == 'content__list':
            total_count = int(
                response.xpath('/html/body/div[3]/div[1]/div[5]/div[1]/p/span[1]/text()').extract()[0])  # 房屋总数
            _ = re.split(r'(rco31rba\d+rea\d+brp\d+erp\d+)', response.url)
            if total_count > 3000:
                # 房屋总数超过3000，缩小查询范围
                rba, rea, brp, erp = map(int, re.search(r'rco31rba(\d+)rea(\d+)brp(\d+)erp(\d+)'
                                                        , response.url).group(1, 2, 3, 4))
                if erp == brp:
                    # 租金约束已达到最小，使用所在区约束继续缩小查询范围
                    self.log_file.write('筛选大于3000: ' + response.url + '\n')
                    for region in self.regions:
                        yield scrapy.Request(url=_[0] + f'{region}/' + _[1],
                                             callback=self.parse_region, dont_filter=True)
                else:
                    # 二分法缩小查询范围
                    mid = int((erp + brp) / 2)
                    yield scrapy.Request(url=_[0] + f'rco31rba{rba}rea{rea}brp{brp}erp{mid}',
                                         callback=self.parse_price, dont_filter=True)
                    yield scrapy.Request(url=_[0] + f'rco31rba{rba}rea{rea}brp{mid + 1}erp{erp}',
                                         callback=self.parse_price, dont_filter=True)
            else:
                # 房屋数量少于3000，爬取每一页的数据
                for i in range(1, int((total_count - 1) / 30) + 2):
                    yield scrapy.Request(url=_[0] + f'pg{i}' + _[1], callback=self.parse_item, dont_filter=True)

    def parse_region(self, response):
        # 判断查询结果是否为 0
        if response.xpath('/html/body/div[3]/div[1]/div[5]/div[1]/div[1]/@class').extract()[0] == 'content__list':
            total_count = int(
                response.xpath('/html/body/div[3]/div[1]/div[5]/div[1]/p/span[1]/text()').extract()[0])  # 房屋总数
            if total_count > 3000:
                self.log_file.write('筛选大于3000: ' + response.url + '\n')
            _ = re.split(r'(rco31rba\d+rea\d+brp\d+erp\d+)', response.url)
            # 爬取每一页的数据
            for i in range(1, min(int((total_count - 1) / 30) + 2, 101)):
                yield scrapy.Request(url=_[0] + f'pg{i}' + _[1], callback=self.parse_item, dont_filter=True)

    def parse_item(self, response):
        rea = int(re.search(r'rea(\d+)', response.url).group(1))  # 面积上限
        item = RentItem()
        for house in response.xpath("/html/body/div[3]/div[1]/div[5]/div[1]/div[1]/div[*]/div"):
            try:
                item['id'] = house.xpath('../@data-house_code').extract()[0]
                item['type'] = house.xpath('p[1]/a/text()').extract()[0].lstrip('\n ').split('·')[0]

                if item['type'] != '独栋':
                    # 整租、合租和无显式房屋类型
                    item['area'] = float(re.search(r'\d+.\d+', house.xpath('p[2]/text()').extract()[4])[0])
                    # 避免由于查询下一个面积范围时下限与此次上限相同导致的多余运算，跳过面积为上限的数据
                    if item['area'] == rea:
                        continue

                    item['region'] = house.xpath('p[2]/a[1]/text()').extract()[0]
                    _ = house.xpath('p[2]/a[2]/text()').extract()
                    # 判断板块是否为空
                    if len(_) == 0:
                        item['plate'] = None
                    else:
                        item['plate'] = _[0]
                    item['location'] = house.xpath('p[2]/a[3]/text()').extract()[0]
                    item['facing'] = house.xpath('p[2]/text()').extract()[5].strip('\n ')
                    item['price_info'] = int(house.xpath('span/em/text()').extract()[0])

                    room_type = house.xpath('p[2]/text()').extract()[6]
                    # 房间数量存在两种表示格式
                    type1 = re.search(r'(\d+|未知)室(\d+)厅(\d+)卫', room_type)
                    type2 = re.search(r'(\d+)房间(\d+)卫', room_type)
                    if type1 is not None:
                        if type1[1] == '未知':
                            item['room_num'] = None
                        else:
                            item['room_num'] = int(type1[1])
                        item['hall_num'], item['toilet_num'] = map(int, type1.group(2, 3))
                    else:
                        item['room_num'], item['toilet_num'] = map(int, type2.group(1, 2))
                        item['hall_num'] = 0
                    yield item

                else:
                    # 独栋
                    # 判断是否存在“仅剩x间”的字段
                    if len(house.xpath('p[2]/span').extract()) != 0:
                        areas = list(map(float, re.findall(r'\d+.\d+', house.xpath('p[2]/text()').extract()[2])))
                        room_type = house.xpath('p[2]/text()').extract()[4]
                    else:
                        areas = list(map(float, re.findall(r'\d+.\d+', house.xpath('p[2]/text()').extract()[0])))
                        room_type = house.xpath('p[2]/text()').extract()[2]
                    # 避免由于查询下一个面积范围时下限与此次上限相同导致的多余运算，跳过面积为上限的数据
                    if areas[len(areas) - 1] == rea:
                        continue

                    item['region'] = None
                    item['plate'] = None
                    item['location'] = house.xpath('p[4]/span[1]/text()').extract()[0].strip('\n ')
                    item['facing'] = None
                    prices = list(map(int, re.findall(r'\d+', house.xpath('span/em/text()').extract()[0])))

                    # 房间数量存在两种表示格式
                    type1 = re.search(r'(\d+|未知)室(\d+)厅(\d+)卫', room_type)
                    type2 = re.search(r'(\d+)房间(\d+)卫', room_type)
                    if type1 is not None:
                        if type1[1] == '未知':
                            item['room_num'] = None
                        else:
                            item['room_num'] = int(type1[1])
                        item['hall_num'], item['toilet_num'] = map(int, type1.group(2, 3))
                    else:
                        item['room_num'], item['toilet_num'] = map(int, type2.group(1, 2))
                        item['hall_num'] = 0

                    # 若面积或租金为一个范围，将最大面积和最大租金以及最小面积和最小租金分别存储为两个房屋
                    item['area'], item['price_info'] = min(areas), min(prices)
                    yield item
                    if len(areas) > 1 or len(prices) > 1:
                        item['area'], item['price_info'] = max(areas), max(prices)
                        item['id'] += "_2"  # 修改第二个房屋的 id 字段，避免后续去重时意外删除
                        yield item

            except Exception as err:
                self.log_file.write(str(err) + ' ' + response.url + '\n')
                continue


# 北京爬虫
class BeijingSpider(Spider):
    name = 'lianjia_beijing'
    allowed_domains = ["bj.lianjia.com/"]
    custom_settings = {
        'ITEM_PIPELINES': {'lianjia_data_process.pipelines.BeijingPipeline': 100}
    }
    start_urls = [f'https://bj.lianjia.com/zufang/rco31rba{Spider.start_area}rea{Spider.end_area}']

    def __init__(self):
        super().__init__()


# 上海爬虫
class ShanghaiSpider(Spider):
    name = 'lianjia_shanghai'
    allowed_domains = ["sh.lianjia.com/"]
    custom_settings = {
        'ITEM_PIPELINES': {'lianjia_data_process.pipelines.ShanghaiPipeline': 200}
    }
    start_urls = [f'https://sh.lianjia.com/zufang/rco31rba{Spider.start_area}rea{Spider.end_area}']

    def __init__(self):
        super().__init__()


# 广州爬虫
class GuangzhouSpider(Spider):
    name = 'lianjia_guangzhou'
    allowed_domains = ["gz.lianjia.com/"]
    custom_settings = {
        'ITEM_PIPELINES': {'lianjia_data_process.pipelines.GuangzhouPipeline': 300}
    }
    start_urls = [f'https://gz.lianjia.com/zufang/rco31rba{Spider.start_area}rea{Spider.end_area}']

    def __init__(self):
        super().__init__()
        self.regions = ['tianhe', 'yuexiu', 'liwan', 'haizhu', 'panyu', 'baiyun', 'huangpugz', 'conghua',
                        'zengcheng', 'huadou', 'nansha']  # 城市所有区，用于划分查询条件


# 深圳爬虫
class ShenzhenSpider(Spider):
    name = 'lianjia_beijing'
    allowed_domains = ["sz.lianjia.com/"]
    custom_settings = {
        'ITEM_PIPELINES': {'lianjia_data_process.pipelines.ShenzhenPipeline': 400}
    }
    start_urls = [f'https://sz.lianjia.com/zufang/rco31rba{Spider.start_area}rea{Spider.end_area}']

    def __init__(self):
        super().__init__()
        self.regions = ['luohuqu', 'futianqu', 'nanshanqu', 'yantianqu', 'baoanqu', 'longgangqu', 'longhuaqu',
                        'guangmingqu', 'pingshanqu', 'dapengxinqu']  # 城市所有区，用于划分查询条件


# 泉州爬虫
class QuanzhouSpider(Spider):
    name = 'lianjia_quanzhou'
    allowed_domains = ["quanzhou.lianjia.com/"]
    custom_settings = {
        'ITEM_PIPELINES': {'lianjia_data_process.pipelines.QuanzhouPipeline': 500}
    }
    start_urls = [f'https://quanzhou.lianjia.com/zufang/rco31rba{Spider.start_area}rea{Spider.end_area}']

    def __init__(self):
        super().__init__()
