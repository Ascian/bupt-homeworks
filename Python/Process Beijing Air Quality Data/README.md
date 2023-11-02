# 数据预处理和可视化作业-2

处理北京质量空气数据（数据源[PM2.5 Data of Five Chinese Cities | Kaggle](https://www.kaggle.com/datasets/uciml/pm25-data-for-five-chinese-cities))

- [数据预处理](#数据预处理)
- [DEWP和TEMP归一化散点图](#dewp和temp归一化散点图)
- [北京空气质量数据离散化](#北京空气质量数据离散化)


## 数据预处理

使用`pandas`函数`read_csv`从源文件`BeijingPM20100101_20151231.csv`中读取所有数据，得到`DataFrame`对象`self.__data`

使用函数`interpolate`对HUMI、PRES、TEMP三列做线性插值处理

    self.__data[['HUMI', 'PRES', 'TEMP']] = self.__data[['HUMI', 'PRES', 'TEMP']].interpolate()

使用函数`loc`选取这三列数据中超过三倍标准差的行，将其修改为三倍标准差的数值

     for label in ['HUMI', 'PRES', 'TEMP']:
        std = self.__data[label].std()
        avg = self.__data[label].mean()
        self.__data.loc[self.__data[label] < avg - 3 * std, label] = avg - 3 * std
        self.__data.loc[self.__data[label] > avg + 3 * std, label] = avg + 3 * std

选取PM_Dongsihuan、 PM_Nongzhanguan三列中超过500的行，并将其修改为500

    for label in ['PM_Dongsi', 'PM_Dongsihuan', 'PM_Nongzhanguan']:
        self.__data.loc[self.__data[label] > 500, label] = 500

使用函数`replace`，并设置`method='bfill'`，对列cbwd中值为'cv'或'NA'的数据将其替换为后项合法数据

    self.__data["cbwd"].replace(['cv', np.nan], method='bfill', inplace=True)

依据公式$x'=\frac{x - minA}{maxA - minA}$计算列DEWP和TEMP0-1归一化后的数值，并新建两列DEWP_minmax和TEMP_minmax存储

    dmin = self.__data[['DEWP', 'TEMP']].min()
    dmax = self.__data[['DEWP', 'TEMP']].max()
    self.__data[['DEWP_minmax', 'TEMP_minmax']] = (self.__data[['DEWP', 'TEMP']] - dmin) / (dmax - dmin)

依据公式$x'=\frac{x - \mu}{\sigma}$计算列DEWP和TEMPZ-score归一化后的数值，并新建两列DEWP_standard和TEMP_standar存储

    std = self.__data[['DEWP', 'TEMP']].std()
    avg = self.__data[['DEWP', 'TEMP']].mean()
    self.__data[['DEWP_standard', 'TEMP_standard']] = (self.__data[['DEWP', 'TEMP']] - avg) / std

预处理完成后的数据存储在文件`beijing.csv`


## DEWP和TEMP归一化散点图

以下是对DEWP和TEMP两列分别进行0-1归一化及Z-Score归一化处理的结果：

![0-1归一化](output/MinMaxScaler.png) 

![Z-Score归一化](output/StandardScaler.png)

## 北京空气质量数据离散化

首先，将四个采样点中非nan的PM2.5浓度的均值作为该时刻的PM2.5浓度。若该时刻均无非nan的数据，则该时刻PM2.5浓度设为nan。

    api['PM'] = self.__data[['PM_Dongsi', 'PM_Dongsihuan', 'PM_Nongzhanguan', 'PM_US Post']].mean(axis=1)

然后，将当天24小时非nan的PM2.5浓度的均值作为该天的PM2.5浓度。使用函数`groupby`按年月日分组，聚合函数采用`mean`求均值。若该天均无nan的数据，则该天PM2.5浓度设为nan。

    api = api.groupby(['year', 'month', 'day'], as_index=False).agg({'PM': 'mean'})

随后按照空气指数相关规定(数据来源于[维基百科](https://zh.wikipedia.org/wiki/%E7%A9%BA%E6%B0%94%E8%B4%A8%E9%87%8F%E6%8C%87%E6%95%B0))，依据PM2.5浓度对应的空气质量指数API，划分为以下六级：

* 第一级(0-35$\mu g/m^3$)
* 第二级(36-75$\mu g/m^3$)
* 第三级(76-115$\mu g/m^3$)
* 第四级(116-150$\mu g/m^3$)
* 第五级(151-250$\mu g/m^3$)
* 第六级(>250$\mu g/m^3$)

按年月进行分组，计算每年每月不同等级空气指数的天数以及值为nan的天数

    api = api.groupby(['year', 'month'], as_index=False).agg({
        'first_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] <= 35)].index.size,
        'second_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] > 35) & (api['PM'] <= 75)].index.size,
        'third_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] > 75) & (api['PM'] <= 115)].index.size,
        'fourth_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] > 115) & (api['PM'] <= 150)].index.size,
        'fifth_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] > 150) & (api['PM'] <= 250)].index.size,
        'sixth_level': lambda x: api[(api.index.isin(x.index)) & (api['PM'] > 250)].index.size,
        'invalid_record': lambda x: api[(api.index.isin(x.index)) & api['PM'].isna()].index.size})

最后列出总的不同等级空气指数的天数

    api.loc['total'] = api.sum()

离散化结果保存在文件`beijing_api.csv`
