#!/user/bin/env python3
# -*- coding: utf-8 -*-

from air_quality import AirQuality

csv_file_path = "BeijingPM20100101_20151231.csv"
new_csv_file_path = "beijing.csv"
min_max_png_path = "MinMaxScaler.png"
standard_png_path = "StandardScaler.png"
api_path = "beijing_api.csv"

if __name__ == '__main__':
    air_quality = AirQuality(csv_file_path)
    air_quality.preprocess()  # 预处理数据
    air_quality.write_to_csv(new_csv_file_path)
    air_quality.draw_temp_dewp_scatter(min_max_png_path, ptype='minmax')  # 0-1归一化散点图
    air_quality.draw_temp_dewp_scatter(standard_png_path, ptype='standard')  # z-score归一化散点图
    air_quality.api_classify(api_path)  # 空气质量离散化

