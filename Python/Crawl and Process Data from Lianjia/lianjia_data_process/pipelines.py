import json

from global_var import beijing_rent
from global_var import shanghai_rent
from global_var import guangzhou_rent
from global_var import shenzhen_rent
from global_var import quanzhou_rent


class BeijingPipeline(object):
    def open_spider(self, spider):
        try:
            self.file = open('../data/beijing.json', "w", encoding="utf-8")
        except Exception as err:
            print(err)

    def process_item(self, item, spider):
        dict_item = dict(item)
        beijing_rent.append(dict_item)
        json_str = json.dumps(dict_item, ensure_ascii=False) + "\n"  # 生成json串
        self.file.write(json_str)
        return item

    def close_spider(self, spider):
        self.file.close()


class ShanghaiPipeline(object):
    def open_spider(self, spider):
        try:
            self.file = open('../data/shanghai.json', "w", encoding="utf-8")
        except Exception as err:
            print(err)

    def process_item(self, item, spider):
        dict_item = dict(item)
        shanghai_rent.append(dict_item)
        json_str = json.dumps(dict_item, ensure_ascii=False) + "\n"  # 生成json串
        self.file.write(json_str)
        return item

    def close_spider(self, spider):
        self.file.close()


class GuangzhouPipeline(object):
    def open_spider(self, spider):
        try:
            self.file = open('../data/guangzhou.json', "w", encoding="utf-8")
        except Exception as err:
            print(err)

    def process_item(self, item, spider):
        dict_item = dict(item)
        guangzhou_rent.append(dict_item)
        json_str = json.dumps(dict_item, ensure_ascii=False) + "\n"  # 生成json串
        self.file.write(json_str)
        return item

    def close_spider(self, spider):
        self.file.close()


class ShenzhenPipeline(object):
    def open_spider(self, spider):
        try:
            self.file = open('../data/shenzhen.json', "w", encoding="utf-8")
        except Exception as err:
            print(err)

    def process_item(self, item, spider):
        dict_item = dict(item)
        shenzhen_rent.append(dict_item)
        json_str = json.dumps(dict_item, ensure_ascii=False) + "\n"  # 生成json串
        self.file.write(json_str)
        return item

    def close_spider(self, spider):
        self.file.close()


class QuanzhouPipeline(object):
    def open_spider(self, spider):
        try:
            self.file = open('../data/quanzhou.json', "w", encoding="utf-8")
        except Exception as err:
            print(err)

    def process_item(self, item, spider):
        dict_item = dict(item)
        quanzhou_rent.append(dict_item)
        json_str = json.dumps(dict_item, ensure_ascii=False) + "\n"  # 生成json串
        self.file.write(json_str)
        return item

    def close_spider(self, spider):
        self.file.close()