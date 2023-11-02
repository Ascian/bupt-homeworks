#!/user/bin/env python3
# -*- coding: utf-8 -*-

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


class AirQuality:
    def __init__(self, csv_file_path):
        self.__data = pd.read_csv(csv_file_path)

    def write_to_csv(self, csv_file_path):
        self.__data.to_csv(csv_file_path, na_rep='NA', index=False)

    # 对数据进行预处理
    def preprocess(self):
        # 对HUMI、PRES、TEMP三列，进行线性插值处理
        self.__data[['HUMI', 'PRES', 'TEMP']] = self.__data[['HUMI', 'PRES', 'TEMP']].interpolate()

        # 对其中超过3倍标准差的高度异常数据，修改为3倍标准差的数值
        for label in ['HUMI', 'PRES', 'TEMP']:
            std = self.__data[label].std()
            avg = self.__data[label].mean()
            self.__data.loc[self.__data[label] < avg - 3 * std, label] = avg - 3 * std
            self.__data.loc[self.__data[label] > avg + 3 * std, label] = avg + 3 * std

        # 对PM_Dongsi、PM_Dongsihuan、PM_Nongzhanguan三列中超过500的数据，修改为500PM 指数
        for label in ['PM_Dongsi', 'PM_Dongsihuan', 'PM_Nongzhanguan']:
            self.__data.loc[self.__data[label] > 500, label] = 500

        # 修改cbwd列中值为“cv”的单元格，其值用后项数据填充
        self.__data["cbwd"].replace(['cv', np.nan], method='bfill', inplace=True)

        # 对DEWP和TEMP两列，进行0-1 归一化
        dmin = self.__data[['DEWP', 'TEMP']].min()
        dmax = self.__data[['DEWP', 'TEMP']].max()
        self.__data[['DEWP_minmax', 'TEMP_minmax']] = (self.__data[['DEWP', 'TEMP']] - dmin) / (dmax - dmin)

        # 对DEWP和TEMP两列，进行Z-Score 归一化
        std = self.__data[['DEWP', 'TEMP']].std()
        avg = self.__data[['DEWP', 'TEMP']].mean()
        self.__data[['DEWP_standard', 'TEMP_standard']] = (self.__data[['DEWP', 'TEMP']] - avg) / std

    # 绘制TEMP-DEWP散点图
    def draw_temp_dewp_scatter(self, png_path, *, ptype='origin'):
        fig, ax = plt.subplots()
        match ptype:
            case 'origin':
                ax.scatter(self.__data['TEMP'], self.__data['DEWP'])
                ax.set_title("Origin")
            case 'minmax':
                ax.scatter(self.__data['TEMP_minmax'], self.__data['DEWP_minmax'])
                ax.set_title("MinMaxScaler")
            case 'standard':
                ax.scatter(self.__data['TEMP_standard'], self.__data['DEWP_standard'])
                ax.set_title("StandardScaler")

        ax.set_xlabel("TEMP")
        ax.set_ylabel("DEMP")
        plt.savefig(png_path)

    # 将北京的空气质量数据进行离散化，按照空气质量指数分级标准，计算出每个级别（或颜色值）对应的天数各有多少
    def api_classify(self, file_path):
        api = self.__data[['year', 'month', 'day']].copy()
        # 将四个地点PM指数平均值作为北京PM指数
        api['PM'] = self.__data[['PM_Dongsi', 'PM_Dongsihuan', 'PM_Nongzhanguan', 'PM_US Post']].mean(axis=1)
        # 将一天内所有采样均值作为一天的PM指数
        api = api.groupby(['year', 'month', 'day'], as_index=False).agg({'PM': 'mean'})

        api[['first_level', 'second_level', 'third_level', 'fourth_level', 'fifth_level', 'sixth_level',
             'invalid_record']] = 0
        # 计算每个月不同级别api的天数
        # 第一级(PM0-35); 第二级(PM36-75); 第三级(PM76-115); 第四级(PM116-150); 第五级(PM151-250); 第六级(PM251-)
        api = api.groupby(['year', 'month'], as_index=False).agg({
            'first_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] <= 35)].index.size,
            'second_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] > 35) & (api['PM'] <= 75)].index.size,
            'third_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] > 75) & (api['PM'] <= 115)].index.size,
            'fourth_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] > 115) & (api['PM'] <= 150)].index.size,
            'fifth_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] > 150) & (api['PM'] <= 250)].index.size,
            'sixth_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] > 250)].index.size,
            'invalid_record': lambda x: api[(api.index.isin(x.index)) & api['PM'].isna()].index.size})
        api.loc['total'] = api.sum()
        api.loc['total', 'year'] = 'total'
        api.loc['total', 'month'] = np.nan
        api.to_csv(file_path, index=False)
