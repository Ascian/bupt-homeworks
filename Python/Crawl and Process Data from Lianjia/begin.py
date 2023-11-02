import json

from scrapy.crawler import CrawlerRunner
from scrapy.utils.log import configure_logging
from twisted.internet import reactor

from global_var import beijing_rent
from global_var import shanghai_rent
from global_var import guangzhou_rent
from global_var import shenzhen_rent
from global_var import quanzhou_rent
from lianjia_data_process.spider import BeijingSpider
from lianjia_data_process.spider import ShanghaiSpider
from lianjia_data_process.spider import GuangzhouSpider
from lianjia_data_process.spider import ShenzhenSpider
from lianjia_data_process.spider import QuanzhouSpider
from data_process import DataProcessor

# 爬取数据
configure_logging()
runner = CrawlerRunner()
runner.crawl(BeijingSpider)
runner.crawl(ShanghaiSpider)
runner.crawl(ShenzhenSpider)
runner.crawl(GuangzhouSpider)
runner.crawl(QuanzhouSpider)
d = runner.join()
d.addBoth(lambda _: reactor.stop())
reactor.run()

# 使用爬好的数据
# beijing_rent = []
# shanghai_rent = []
# guangzhou_rent = []
# shenzhen_rent = []
# quanzhou_rent = []
# with open('../data/beijing.json', 'r', encoding="utf-8") as json_file:
#     for line in json_file.readlines():
#         beijing_rent.append(json.loads(line))
# with open('../data/shanghai.json', 'r', encoding="utf-8") as json_file:
#     for line in json_file.readlines():
#         shanghai_rent.append(json.loads(line))
# with open('../data/guangzhou.json', 'r', encoding="utf-8") as json_file:
#     for line in json_file.readlines():
#         guangzhou_rent.append(json.loads(line))
# with open('../data/shenzhen.json', 'r', encoding="utf-8") as json_file:
#     for line in json_file.readlines():
#         shenzhen_rent.append(json.loads(line))
# with open('../data/quanzhou.json', 'r', encoding="utf-8") as json_file:
#     for line in json_file.readlines():
#         quanzhou_rent.append(json.loads(line))

# 数据分析与展示
data_processor = DataProcessor(beijing=beijing_rent, shanghai=shanghai_rent, guangzhou=guangzhou_rent,
                               shenzhen=shenzhen_rent, quanzhou=quanzhou_rent)
data_processor.total_rent_bar()  # 总体租金情况直方图
data_processor.total_rent_per_area_bar()  # 总体单位面积租金情况
data_processor.rent_of_different_room_num_bar()  # 一居、二居、三居租金情况
data_processor.rent_of_different_plate_bar()  # 所有板块租金均价情况
data_processor.rent_per_area_of_different_facing_eventplot()  # 不同朝向单位面积租金分布情况
data_processor.gdp_rent_per_area_violinplot()  # 人均GDP和单位面积租金分布的关系
data_processor.average_salary_rent_per_area_violinplot()  # 平均工资和单位面积租金分布的关系
