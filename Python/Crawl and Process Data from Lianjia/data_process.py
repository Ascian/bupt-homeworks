import json

import pandas as pd
import matplotlib
import matplotlib.pyplot as plt


# 对数据进行
class DataProcessor:
    # 对数据进行预处理
    def __init__(self, *, beijing, shanghai, guangzhou, shenzhen, quanzhou):
        matplotlib.rcParams['font.sans-serif'] = ['SimHei']
        # 依据 id 字段对数据进行去重处理，并存储为 DataFrame
        self.__data = {'北京': pd.DataFrame(data=beijing).drop_duplicates(subset=['id'], ignore_index=True),
                       '上海': pd.DataFrame(data=shanghai).drop_duplicates(subset=['id'], ignore_index=True),
                       '广州': pd.DataFrame(data=guangzhou).drop_duplicates(subset=['id'], ignore_index=True),
                       '深圳': pd.DataFrame(data=shenzhen).drop_duplicates(subset=['id'], ignore_index=True),
                       '泉州': pd.DataFrame(data=quanzhou).drop_duplicates(subset=['id'], ignore_index=True)}

        # 去除异常值
        all_data = pd.concat(self.__data.values())
        area_per_room = all_data['area'] / (all_data['room_num'] + all_data['hall_num'] + all_data['toilet_num'])
        mean = area_per_room.mean()
        std = area_per_room.std()

        # 绘制面积 - 房间数比值离散直方图
        # fig, ax = plt.subplots(figsize=(15, 10))
        # for i in range(int(area_per_room.min()), int(area_per_room.max()) + 1):
        #     ax.bar(i, area_per_room[(area_per_room >= i) & (area_per_room < i + 1)].size)
        # ax.set(title="面积-房间数比值离散直方图", ylabel="数量(个)", xlabel="面积-房间数比值")
        # plt.xlim(0, 100)
        # plt.savefig("../img/面积 - 房间数比值离散直方图.png", dpi=300)
        # plt.show()

        # 绘制面积 - 房间数比值饼图，观察与平均值的偏差不超过一倍、两倍、三倍或超过三倍标准差的数据占比
        # fig, ax = plt.subplots(figsize=(8, 8))
        # labels = '$(μ-σ,μ+σ)$', r'$(μ-2σ,μ-σ)\vee(μ+σ,μ+2σ)$', r'$(μ-3σ,μ-2σ)\vee(μ+2σ,μ+3σ)$', \
        #          r'$(-\infty,μ-3σ)\vee(μ+3σ,+\infty)$'
        # explode = (0, 0, 0.1, 0.2)
        # _ = [len(area_per_room[(area_per_room >= mean - std) &
        #                        (area_per_room <= mean + std)]),
        #      len(area_per_room[(area_per_room >= mean - 2 * std) &
        #                        (area_per_room < mean - std) |
        #                        (area_per_room > mean + std) &
        #                        (area_per_room <= mean + 2 * std)]),
        #      len(area_per_room[(area_per_room >= mean - 3 * std) &
        #                        (area_per_room < mean - 2 * std) |
        #                        (area_per_room > mean + 2 * std) &
        #                        (area_per_room <= mean + 3 * std)]),
        #      len(area_per_room[(area_per_room < mean - 3 * std) |
        #                        (area_per_room > mean + 3 * std)]),
        #      ]
        # ax.pie(_, explode=explode, labels=labels, autopct='%1.2f%%', startangle=90)
        # plt.savefig("../img/面积 - 房间数比值饼图.png", dpi=300)
        # plt.show()

        # 对于每个城市去除与平均值的偏差超过三倍标准差的数据。
        for city_data in self.__data.values():
            city_data['area_per_room'] = city_data['area'] / (city_data['room_num'] + city_data['hall_num'] +
                                                              city_data['toilet_num'])
            city_data.drop(city_data[(city_data['area_per_room'] < mean - 3 * std) |
                                     (city_data['area_per_room'] > mean + 3 * std)].index, inplace=True)
            city_data.drop(columns=['area_per_room'], inplace=True)

    # 比较 5 个城市的总体房租情况，包含租金的均价、最高价、最低价、中位数等信息
    def total_rent_bar(self):
        fig, ax = plt.subplots(2, 2, figsize=(9, 9))
        i = 0
        # 展示每个城市的租金的均价、最高价、最低价、中位数
        for city in self.__data.values():
            rent_max = round(city['price_info'].max())
            rent_min = round(city['price_info'].min())
            rent_avg = round(city['price_info'].mean())
            rent_med = round(city['price_info'].median())
            ax[0, 0].bar(i, rent_max, color="royalblue")
            ax[0, 0].text(i, rent_max, rent_max, ha='center', va='bottom')
            ax[0, 1].bar(i, rent_min, color="royalblue")
            ax[0, 1].text(i, rent_min, rent_min, ha='center', va='bottom')
            ax[1, 0].bar(i, rent_avg, color="royalblue")
            ax[1, 0].text(i, rent_avg, rent_avg, ha='center', va='bottom')
            ax[1, 1].bar(i, rent_med, color="royalblue")
            ax[1, 1].text(i, rent_med, rent_med, ha='center', va='bottom')
            i += 1
        ax[0, 0].set(title="租金最高价直方图", ylabel="最高价(元/月)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        ax[0, 1].set(title="租金最低价直方图", ylabel="最低价(元/月)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        ax[1, 0].set(title="租金均价直方图", ylabel="均价(元/月)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        ax[1, 1].set(title="租金中位数直方图", ylabel="中位数(元/月)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        plt.savefig("../img/总体租金情况.png", dpi=300)
        plt.show()

    # 比较 5 个城市的总体房租情况，包含单位面积租金（元/平米）的均价、最高价、最低价、中位数
    def total_rent_per_area_bar(self):
        fig, ax = plt.subplots(2, 2, figsize=(9, 9))
        i = 0
        # 展示每个城市的单位面积租金（元/平米）的均价、最高价、最低价、中位数
        for city in self.__data.values():
            rent_per_area = city['price_info'] / city['area']
            rent_per_area_max = round(rent_per_area.max())
            rent_per_area_min = round(rent_per_area.min())
            rent_per_area_avg = round(rent_per_area.mean())
            rent_per_area_med = round(rent_per_area.median())
            ax[0, 0].bar(i, rent_per_area_max, color="royalblue")
            ax[0, 0].text(i, rent_per_area_max, rent_per_area_max, ha='center', va='bottom')
            ax[0, 1].bar(i, rent_per_area_min, color="royalblue")
            ax[0, 1].text(i, rent_per_area_min, rent_per_area_min, ha='center', va='bottom')
            ax[1, 0].bar(i, rent_per_area_avg, color="royalblue")
            ax[1, 0].text(i, rent_per_area_avg, rent_per_area_avg, ha='center', va='bottom')
            ax[1, 1].bar(i, rent_per_area_med, color="royalblue")
            ax[1, 1].text(i, rent_per_area_med, rent_per_area_med, ha='center', va='bottom')
            i += 1
        ax[0, 0].set(title="单位面积租金最高价直方图", ylabel="最高价(元/平米)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        ax[0, 1].set(title="单位面积租金最低价直方图", ylabel="最低价(元/平米)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        ax[1, 0].set(title="单位面积租金均价直方图", ylabel="均价(元/平米)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        ax[1, 1].set(title="单位面积租金中位数直方图", ylabel="中位数(元/平米)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        plt.savefig("../img/总体单位面积租金情况.png", dpi=300)
        plt.show()

    # 比较 5 个城市一居、二居、三居的情况，包含租金的均价、最高价、最低价、中位数
    def rent_of_different_room_num_bar(self):
        fig, ax = plt.subplots(2, 2, figsize=(18, 10))
        i = 0
        # 展示每个城市一居、二居、三居的情况
        for city in self.__data.values():
            # 由于合租房屋房间数标的是多人合租的房间总数，因此需要删除所有合租房屋数据
            city_data = city.drop(city[city['type'] == '合租'].index)
            # 分别对于一居、二居、三居，展示租金的均价、最高价、最低价、中位数
            for room_num, room_type, room_color in ((1, '一居', 'dodgerblue'), (2, '二居', 'royalblue'),
                                                    (3, '三居', 'mediumblue')):
                rent = city_data['price_info'][city_data['room_num'] == room_num]
                rent_max = round(rent.max())
                rent_min = round(rent.min())
                rent_avg = round(rent.mean())
                rent_med = round(rent.median())
                width = 0.25  # 直方图柱宽
                offset = (room_num - 2) * width
                ax[0, 0].bar(i + offset, rent_max, color=room_color, width=width, label=room_type, edgecolor='white')
                ax[0, 0].text(i + offset, rent_max, rent_max, ha='center', va='bottom')
                ax[0, 1].bar(i + offset, rent_min, color=room_color, width=width, label=room_type, edgecolor='white')
                ax[0, 1].text(i + offset, rent_min, rent_min, ha='center', va='bottom')
                ax[1, 0].bar(i + offset, rent_avg, color=room_color, width=width, label=room_type, edgecolor='white')
                ax[1, 0].text(i + offset, rent_avg, rent_avg, ha='center', va='bottom')
                ax[1, 1].bar(i + offset, rent_med, color=room_color, width=width, label=room_type, edgecolor='white')
                ax[1, 1].text(i + offset, rent_med, rent_med, ha='center', va='bottom')
            i += 1
        ax[0, 0].set(title="租金最高价直方图", ylabel="最高价(元/月)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        ax[0, 1].set(title="租金最低价直方图", ylabel="最低价(元/月)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        ax[1, 0].set(title="租金均价直方图", ylabel="均价(元/月)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        ax[1, 1].set(title="租金中位数直方图", ylabel="中位数(元/月)", xticks=range(len(self.__data)),
                     xticklabels=self.__data.keys())
        for i in ((0, 0), (0, 1), (1, 0), (1, 1)):
            ax[i].legend(labels=['一居', '二居', '三居'])
        plt.savefig("../img/一居、二居、三居租金情况.png", dpi=300)
        plt.show()

    # 计算和分析每个城市不同板块的均价情况
    def rent_of_different_plate_bar(self):
        for city_name, city_data in self.__data.items():
            # 将每个城市数据按照板块分组，并计算每个版块的租金均价
            plate_data = city_data.groupby(by=['plate'], sort=False, as_index=False).agg({'region': lambda x: x.iat[0],
                                                                                          'price_info': 'mean'})
            # 为了查询板块时能够快速的找到想要找的板块，数据应当按照所在区排序，对于同一个区内的板块再按照租金均价大小排序
            plate_data.sort_values(by=['region', 'price_info'], inplace=True, ignore_index=True)

            # 为了查询板块时能够快速的找到想要找的板块，对于每一个区的所有板块应当使用不同颜色表示
            regions = plate_data['region'].drop_duplicates()
            cmap = plt.get_cmap(name='turbo')
            region_colors = {regions.iat[i]: cmap(i / (regions.size - 1)) for i in range(regions.size)}

            fig, ax = plt.subplots(figsize=(plate_data.index.size * 0.22, 15))
            for region in regions:
                region_plate_data = plate_data.loc[plate_data['region'] == region]
                ax.bar(region_plate_data.index, region_plate_data['price_info'], label=region,
                       color=region_colors[region])
                for index, data in zip(region_plate_data.index, region_plate_data['price_info']):
                    ax.text(index, data, round(data), ha='center', va='bottom', fontsize=4.5)

            ax.set(title=f"{city_name}不同板块租金均价直方图", ylabel="均价(元/月)", xticks=range(plate_data.index.size))
            # 在板块名的每个字符后面加上换行符，使得板块名能够竖向显示
            plate_name = ['\n'.join(list(name)) for name in plate_data['plate']]
            ax.set_xticklabels(plate_name, fontsize=10)
            ax.yaxis.set_major_locator(plt.MultipleLocator(1000))  # 设置y轴刻度间隔为1000
            plt.xlim(-1, plate_data.index.size)
            ax.grid(True, linestyle='--', alpha=0.8, axis='y')
            plt.legend()
            plt.savefig(f"../img/{city_name}不同板块租金均价情况.png", dpi=300)
            plt.show()

    # 比较各个城市不同朝向的单位面积租金分布情况
    def rent_per_area_of_different_facing_eventplot(self):
        for city_name, city_data, max_y in zip(self.__data.keys(), self.__data.values(),
                                               (400, 500, 325, 450, 110)):
            fig, ax = plt.subplots(figsize=(15, 20))
            city_data.loc[city_data['facing'].isnull(), 'facing'] = ''  # 空朝向设置为空字符串
            # 收集该城市所有房屋的所有朝向
            facings = city_data['facing'].drop_duplicates()
            facings = sorted(list({facing for facing_str in facings.tolist() for facing in facing_str.split()}))
            # 为每个房屋添加不同朝向的字段，若该房屋具有该朝向，则该字段设置为True
            for facing in facings:
                city_data[facing] = pd.DataFrame([facing in city_data['facing'][i].split() for i in city_data.index],
                                                 index=city_data.index)

            city_data['rent_per_area'] = city_data['price_info'] / city_data['area']
            # 随后对于每个朝向，分别展示具有该朝向的房屋的单位面积租金
            for facing, i in zip(facings, range(0, len(facings) * 2, 2)):
                rent_per_area = city_data['rent_per_area'][city_data[facing] == True]
                if rent_per_area.size != 0:
                    ax.eventplot(rent_per_area, orientation="vertical", lineoffsets=[i], linewidth=0.5)

            ax.set(title=f"{city_name}不同朝向单位面积租金分布图", ylabel="单位面积租金(元/平米)",
                   xticks=range(0, len(facings) * 2, 2), xticklabels=facings)
            plt.ylim(0, top=max_y)
            plt.savefig(f"../img/{city_name}不同朝向单位面积租金分布情况.png", dpi=300)
            plt.show()

    # 分析并展示各个城市的人均 GDP 和单位面积租金分布的关系
    def gdp_rent_per_area_violinplot(self):
        # 读取各城市 gdp 数据
        with open('../gdp.json', 'r', encoding='utf-8') as json_file:
            gdp = json.load(json_file)
        city_gdp = [gdp[city] for city in self.__data.keys()]
        city_rent_per_area = [self.__data[city]['price_info'] / self.__data[city]['area']
                              for city in self.__data.keys()]

        fig, ax = plt.subplots(figsize=(20, 20))
        parts = ax.violinplot(city_rent_per_area, city_gdp, widths=6000, showextrema=False)
        cmap = plt.get_cmap(name='gist_rainbow')
        for body, city_name, i in zip(parts['bodies'], self.__data.keys(), range(len(parts['bodies']))):
            # 用实心点标出了每个城市单位面积租金的中位数，并将其与点(0, 0)连接
            ax.plot([0, city_gdp[i]], [0, city_rent_per_area[i].median()],
                    color=cmap(i / len(parts['bodies']), 0.4))
            ax.scatter(city_gdp[i], city_rent_per_area[i].median(),
                       color=cmap(i / len(parts['bodies'])))
            body.set_facecolor(cmap(i / len(parts['bodies'])))
            body.set_label(city_name)
            body.set_alpha(0.4)

        ax.set(title="人均GDP和单位面积租金分布关系图", ylabel="单位面积租金(元/平米)", xlabel="人均GDP(元)")
        ax.grid(True, linestyle='--', linewidth=1.5, alpha=0.5, axis='y')
        plt.legend()
        plt.ylim(0, 250)
        plt.xlim(0)
        plt.savefig("../img/人均GDP和单位面积租金分布的关系.png", dpi=300)
        plt.show()

    # 分析并展示各个城市的全口径平均工资 和单位面积租金分布的关系
    def average_salary_rent_per_area_violinplot(self):
        # 读取各城市全口径平均工资
        with open('../average_salary.json', 'r', encoding='utf-8') as json_file:
            average_salary = json.load(json_file)
        city_average_salary = [average_salary[city] for city in self.__data.keys()]
        city_rent_per_area = [self.__data[city]['price_info'] / self.__data[city]['area']
                              for city in self.__data.keys()]
        fig, ax = plt.subplots(figsize=(20, 20))
        parts = ax.violinplot(city_rent_per_area, city_average_salary, widths=400,
                              showextrema=False)
        cmap = plt.get_cmap(name='gist_rainbow')

        for body, city_name, i in zip(parts['bodies'], self.__data.keys(), range(len(parts['bodies']))):
            # 用实心点标出了每个城市单位面积租金的中位数，并将其与点(0, 0)连接
            ax.plot([0, city_average_salary[i]], [0, city_rent_per_area[i].median()],
                    color=cmap(i / len(parts['bodies']), 0.4))
            ax.scatter(city_average_salary[i], city_rent_per_area[i].median(),
                       color=cmap(i / len(parts['bodies'])))
            body.set_facecolor(cmap(i / len(parts['bodies'])))
            body.set_label(city_name)
            body.set_alpha(0.4)

        ax.set(title="平均工资和单位面积租金分布关系图", ylabel="单位面积租金(元/平米)", xlabel="平均工资(元)")
        ax.grid(True, linestyle='--', linewidth=1.5, alpha=0.5, axis='y')
        plt.legend()
        plt.ylim(0, 250)
        plt.xlim(0)
        plt.savefig("../img/平均工资和单位面积租金分布的关系.png", dpi=300)
        plt.show()
