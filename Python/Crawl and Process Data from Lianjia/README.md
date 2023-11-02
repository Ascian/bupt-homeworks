# Python大作业-租房数据分析

本实验抓取链家网北上广深 4 个一线城市和泉州一个 5 个城市的全部租房数据，并对数据进行分析，绘制合适的图表。

## 数据获取

### 数据结构

由于5个城市的租房爬取过程相同，因此可以在`spider.py`中定义一个基类`Spider`并将爬取过程使用的函数定义在基类里。五个城市的爬虫分别继承自基类并定义各自的爬取域名和初始 url。

`item.py`中定义`RentItem`，包含以下字段：

* id：房屋的 data-house_code，用于后面的查重处理
* type：房屋类型，包括整租、合租、独栋
* region：所在区
* plate：所在板块
* location：具体位置
* area：面积
* facing：朝向
* room_num：室数量
* hall_num：厅数量
* toilet_num：卫生间数量
* price_info：租金价格，单位元/月

`pipeline.py`中分别定义五个城市的 pipeline，将数据存入`global_var.py`中定义的全局变量用于后续数据处理，并输出到各自的 json 文件中，用于检查数据或直接用于数据处理。

### 爬取过程

经过测试，链家网的查询结果最多不会超过100页。若尝试修改网址到 10000 页以上，已经远远超出房屋数量的页数依旧会返回数据，暂不清楚超过一百页后的网址返回的是什么样的数据。保险起见，为了爬取一个城市所有的数据，采用设置约束（例如面积，租金或地区）的方式，使得单次的查询结果不会超过 100 页。

首先设置面积约束，采用分治法，二分查询并判断结果是否超过 100 页，即 3000 个房屋数据。例如设置爬取面积下限为 1，爬取面积上限为 10000。首先查询面积为 [1, 10000] 的房屋数据，发现查询结果超过 3000 个房屋数据。随后查找 [1, 5000] 和 [5000, 10000] 的页面。发现面积为 [5000, 10000] 的结果小于 3000 个，可以开始爬取每一页的数据。而面积为 [1, 5000] 的结果依旧大于 3000 个，继续采用二分法缩小查询范围。以下为关键代码实现：

    _ = re.split(r'(rco31rba\d+rea\d+)', response.url)
    if total_count > 3000:
        # 房屋总数超过3000，缩小查询范围
        rba, rea = map(int, re.search(r'rba(\d+)rea(\d+)', response.url).group(1, 2))
        if rea - rba == 1:
            # 面积约束已达到最小，使用租金价格约束继续缩小查询范围
            yield scrapy.Request(url=_[0] + _[1] + f'brp{Spider.start_price}erp{Spider.end_price}', callback=self.parse_price, dont_filter=True)
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

> 由于链家网只能查询整数面积范围，而面积采用小数表示，因此很可能会导致大量的整数面积被重复爬取，例如 [1, 5000] 和 [5000, 10000] 的查询结果就会导致面积为 5000 的房屋数据被重复爬取。因此在分析页面时，应当跳过那些面积等于上限的房屋数据，减少后续的运算，将查询范围改成 [1, 5000) 和 [5000, 10000)。

当面积划分约束已达到最小，即面积上限和面积下限只差 1 时，若结果依旧大于 3000 个，继续采用对租金设置约束的方法，进一步缩小查询范围。对租金设置约束的方式和对面积设置约束的方法几乎相同，唯一不同的点在于，租金都为整数，因此划分可以避免重复，例如 [1, 10000] 应当被划分为 [1, 5000] 和 [5001, 10000] ，不会出现租金为 5000 的房屋数据被重复爬取的现象。以下为关键代码实现：

    _ = re.split(r'(rco31rba\d+rea\d+brp\d+erp\d+)', response.url)
    if total_count > 3000:
        # 房屋总数超过3000，缩小查询范围
        rba, rea, brp, erp = map(int, re.search(r'rco31rba(\d+)rea(\d+)brp(\d+)erp(\d+)' , response.url).group(1, 2, 3, 4))
        if erp == brp:
            # 租金约束已达到最小，使用所在区约束继续缩小查询范围
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

经过爬取测试发现，在爬取深圳租房数据时，即使面积和租金的约束都已经达到最小，依旧存在结果大于 3000 个的情况，因此需要继续划分查询范围。第三层约束采用所在区约束，依次查询每一个区的数据。暂时没有发现存在某个区面积和租金的约束都已经达到最小结果依旧大于 3000 个的情况。

### 特殊情况处理

1. 可能出现查询结果为 0 的情况，需要在分析页面前做判断
    `if response.xpath('/html/body/div[3]/div[1]/div[5]/div[1]/div[1]/@class').extract()[0] == 'content__list'`。
    若页面无数据，该值会等于 'content_empty1'
2. 房屋标题中房屋类型存在四种情况，分别是整租、合租、独栋和无显式房屋类型（实际类型为整租）。其中整租、合租和无显式房屋类型的 html 格式相同，独栋有所区别。因此需要在分析房屋数据前者增加判断`if item['type'] != '独栋'`，对于两种格式分别进行分析
3. 对于独栋类型的房屋，还可能存在“仅剩x间”的字段，含有该字段和不含有该字段的数据格式中面积和房间数量位置不同，需要区别处理。
4. 房屋数据数据中可能出现所在板块为空的情况，例如![板块为空](doc_img/%E6%9D%BF%E5%9D%97%E4%B8%BA%E7%A9%BA.png)，此时将所在板块设置为`None`
5. 房屋的房间数量存在两种表示格式，分别是`(\d+|未知)室(\d+)厅(\d+)卫`和`(\d+)房间(\d+)卫`，其中第一种格式的室数量可能为'未知'，需要将室数量字段设置为`None`。对于第二种格式，没有厅数量，需要设置厅数量为 0
6. 对于独栋类型的房屋，其面积和租金都可能为一个范围，由于不清楚可能存在的中间面积、价格，这里简单将最大面积和最大租金以及最小面积和最小租金分别存储为两个房屋，并修改后一个房屋的 id 字段，加上"_2"，避免后续去重时意外删除

## 数据处理

首先依据 id 字段对数据进行去重处理，并存储为 DataFrame：

    self.__data = {'北京': pd.DataFrame(data=beijing).drop_duplicates(subset=['id'], ignore_index=True),
                    '上海': pd.DataFrame(data=shanghai).drop_duplicates(subset=['id'], ignore_index=True),
                    '广州': pd.DataFrame(data=guangzhou).drop_duplicates(subset=['id'], ignore_index=True),
                    '深圳': pd.DataFrame(data=shenzhen).drop_duplicates(subset=['id'], ignore_index=True),
                    '泉州': pd.DataFrame(data=quanzhou).drop_duplicates(subset=['id'], ignore_index=True)}


然后进行异常值处理。按照常理来讲，一个正常房间的面积大小应当处在一个合理范围内，观察数据时发现多个上千平米的房屋却仅有很少的房间，单个房间的大小就上百平米，属于异常数据。

为了观察单个房间面积的分布，对每个房屋求面积与房间数（包括室数、厅数、卫生间数）的比值，将该比值离散化，记录每个整数区间的房屋数量，最终绘制成直方图：

![面积 - 房间数比值离散直方图](img/%E9%9D%A2%E7%A7%AF%20-%20%E6%88%BF%E9%97%B4%E6%95%B0%E6%AF%94%E5%80%BC%E7%A6%BB%E6%95%A3%E7%9B%B4%E6%96%B9%E5%9B%BE.png)

由于单个房间面积大于 100 的房屋已经非常稀少，因此为了观察方便，设置图像 x 轴最大显示范围为 100。

观察直方图发现除了 0 到 3 区间的数据，其余范围内的数据均符合正态分布。而 0 到 3 区间的数据是由于合租房屋造成的。对于不少合租房屋，房间数标的是多人合租的房间总数，而面积和租金却为个人数据。该类数据仅会对于后续一居、二居、三居的数据展示造成影响，只需在该展示前进行处理即可，其余数据展示均可忽略其房间数将其当做整租看待。

为了进一步验证该猜测，绘制面积-房间数比值饼图，观察与平均值的偏差不超过一倍、两倍、三倍和超过三倍标准差的数据占比：

![面积 - 房间数比值饼图](img/%E9%9D%A2%E7%A7%AF%20-%20%E6%88%BF%E9%97%B4%E6%95%B0%E6%AF%94%E5%80%BC%E9%A5%BC%E5%9B%BE.png)

占比基本符合正态分布，因此应当将与平均值的偏差超过三倍标准差的数据作为异常值删除。以下为异常值处理的代码：

    all_data = pd.concat(self.__data.values())
    area_per_room = all_data['area'] / (all_data['room_num'] + all_data['hall_num'] + all_data['toilet_num'])
    mean = area_per_room.mean()
    std = area_per_room.std()
    # 对于每个城市去除与平均值的偏差超过三倍标准差的数据。
    for city_data in self.__data.values():
        city_data['area_per_room'] = city_data['area'] / (city_data['room_num'] + city_data['hall_num'] + city_data['toilet_num'])
        city_data.drop(city_data[(city_data['area_per_room'] < mean - 3 * std) |
                            (city_data['area_per_room'] > mean + 3 * std)].index, inplace=True)
        city_data.drop(columns=['area_per_room'], inplace=True)


## 数据展示

### 比较 5 个城市的总体房租情况，包含租金的均价、最高价、最低价、中位数

![总体租金情况](img/%E6%80%BB%E4%BD%93%E7%A7%9F%E9%87%91%E6%83%85%E5%86%B5.png)

绘制 4 个子图，分别展示5个城市租金的最高价、最低价、均价和中位数。

从图中可以了解到以下信息：

* 对于租金的最高价：深圳 > 上海 > 广州 > 北京 > 泉州
* 对于租金的最低价：上海 > 北京 > 深圳 > 广州 > 泉州
* 对于租金的均价和中位数：北京 > 上海 > 深圳 > 广州 > 泉州

### 比较 5 个城市的总体房租情况，包含单位面积租金（元/平米）的均价、最高价、最低价、中位数

![总体单位面积租金情况](img/%E6%80%BB%E4%BD%93%E5%8D%95%E4%BD%8D%E9%9D%A2%E7%A7%AF%E7%A7%9F%E9%87%91%E6%83%85%E5%86%B5.png)

绘制 4 个子图，分别展示5个城市单位面积租金的最高价、最低价、均价和中位数，数据都做了取整处理。

从图中可以了解到以下信息：

* 对于单位面积租金的最高价：广州 > 北京 > 深圳 > 上海 > 泉州
* 对于单位面积租金的最低价：北京 > 上海 > 深圳 > 广州 > 泉州
* 对于单位面积租金的均价和中位数：上海 > 北京 > 深圳 > 广州 > 泉州

### 比较 5 个城市一居、二居、三居的情况，包含均价、最高价、最低价、中位数

对于之前提及 0 到 3 区间的异常数据，在该部分需要进行处理。由于合租房屋房间数标的是多人合租的房间总数，因此需要删除所有合租房屋数据:

     city_data = city.drop(city[city['type'] == '合租'].index)
    
后续的数据展示将以处理后的`city_data`为依据。

![一居、二居、三居租金情况](img/%E4%B8%80%E5%B1%85%E3%80%81%E4%BA%8C%E5%B1%85%E3%80%81%E4%B8%89%E5%B1%85%E7%A7%9F%E9%87%91%E6%83%85%E5%86%B5.png)

绘制 4 个子图，分别展示5个城市一居、二居、三居租金的最高价、最低价、均价和中位数。一居、二居、三居分别用不同的颜色表示。单看一个城市可以比较一、二、三居的差距，单看一个颜色可以比较相同房间数不同城市的租金差距。

从图中可以了解到以下信息：

* 除了北京租金的最高价和最低价、上海的最低价、泉州的最低价，其余数据都符合三居 > 二居 > 一居
* 对于租金的最高价
  * 一居：深圳 > 广州 > 北京 > 上海 > 泉州
  * 二居：北京 > 广州 > 深圳 > 上海 > 泉州
  * 三居：深圳 > 广州 > 上海 > 北京 > 泉州
* 对于租金的最低价
  * 一居：北京 = 上海 > 深圳 > 广州 > 泉州
  * 二居：上海 > 深圳 > 北京 > 广州 > 泉州
  * 三居：北京 > 深圳 > 上海 > 广州 > 泉州
* 对于租金的均价
  * 一居和二居：北京 > 上海 > 深圳 > 广州 > 泉州
  * 三居：上海 > 北京 > 深圳 > 广州 > 泉州
* 对于租金的中位数的一居、二居和三居均符合：北京 > 上海 > 深圳 > 广州 > 泉州

### 计算和分析每个城市不同板块的均价情况

首先使用`groupby`函数，将每个城市数据按照板块分组，并计算每个版块的租金均价，同时为了查询板块时能够快速的找到想要找的板块，数据应当按照所在区排序，对于同一个区内的板块再按照租金均价大小排序：

    plate_data = city_data.groupby(by=['plate'], sort=False, as_index=False).agg({'region': lambda x: x.iat[0], 'price_info': 'mean'})
    plate_data.sort_values(by=['region', 'price_info'], inplace=True, ignore_index=True)

同时为了能够快速的找到板块所在区，对于每一个区的所有板块应当使用不同颜色表示。由于每个城市的区数量不同，没法为每个区一一设置颜色，因此可以使用`Colormap`来实现，将每个区映射到每个颜色上：

    regions = plate_data['region'].drop_duplicates()
    cmap = plt.get_cmap(name='turbo')
    region_colors = {regions.iat[i]: cmap(i / (regions.size - 1)) for i in range(regions.size)}

由于数据过于密集，若想显示全板块名，应当在板块名的每个字符后面加上换行符，使得板块名能够竖向显示：

    plate_name = ['\n'.join(list(name)) for name in plate_data['plate']]
    ax.set_xticklabels(plate_name, fontsize=10)

北京不同板块租金均价情况：

![北京不同板块租金均价情况](img/%E5%8C%97%E4%BA%AC%E4%B8%8D%E5%90%8C%E6%9D%BF%E5%9D%97%E7%A7%9F%E9%87%91%E5%9D%87%E4%BB%B7%E6%83%85%E5%86%B5.png)

上海不同板块租金均价情况：

![上海不同板块租金均价情况](img/%E4%B8%8A%E6%B5%B7%E4%B8%8D%E5%90%8C%E6%9D%BF%E5%9D%97%E7%A7%9F%E9%87%91%E5%9D%87%E4%BB%B7%E6%83%85%E5%86%B5.png)

广州不同板块租金均价情况：

![广州不同板块租金均价情况](img/%E5%B9%BF%E5%B7%9E%E4%B8%8D%E5%90%8C%E6%9D%BF%E5%9D%97%E7%A7%9F%E9%87%91%E5%9D%87%E4%BB%B7%E6%83%85%E5%86%B5.png)

深圳不同板块租金均价情况：

![深圳不同板块租金均价情况](img/%E6%B7%B1%E5%9C%B3%E4%B8%8D%E5%90%8C%E6%9D%BF%E5%9D%97%E7%A7%9F%E9%87%91%E5%9D%87%E4%BB%B7%E6%83%85%E5%86%B5.png)

泉州不同板块租金均价情况：

![泉州不同板块租金均价情况](img/%E6%B3%89%E5%B7%9E%E4%B8%8D%E5%90%8C%E6%9D%BF%E5%9D%97%E7%A7%9F%E9%87%91%E5%9D%87%E4%BB%B7%E6%83%85%E5%86%B5.png)

通过这些直方图，能够快速的依据颜色找到板块所在区。同时还可以通过该图观察出各区租金均价的差别，简单的了解到哪个区的租金普遍较高，哪个区的租金普遍较低。

### 比较各个城市不同朝向的单位面积租金分布情况

由于每个房屋可能具有多个朝向，需要将 facing 字段中的朝向分离出来。首先收集该城市所有房屋的所有朝向：

    facings = city_data['facing'].drop_duplicates()
    facings = sorted(list({facing for facing_str in facings.tolist() for facing in facing_str.split()}))

随后为每个房屋添加不同朝向的字段，若该房屋具有该朝向，则该字段设置为 True

    for facing in facings:
        city_data[facing] = pd.DataFrame([facing in city_data['facing'][i].split() for i in city_data.index], index=city_data.index)

最后对于每个朝向，采用`eventplot`分别展示具有该朝向的房屋的单位面积租金:

    for facing, i in zip(facings, range(0, len(facings) * 2, 2)):
        rent_per_area = city_data['rent_per_area'][city_data[facing] == True]
        if rent_per_area.size != 0:
            ax.eventplot(rent_per_area, orientation="vertical", lineoffsets=[i], linewidth=0.5)

北京不同朝向单位面积租金分布情况：

![北京不同朝向单位面积租金分布情况](img/%E5%8C%97%E4%BA%AC%E4%B8%8D%E5%90%8C%E6%9C%9D%E5%90%91%E5%8D%95%E4%BD%8D%E9%9D%A2%E7%A7%AF%E7%A7%9F%E9%87%91%E5%88%86%E5%B8%83%E6%83%85%E5%86%B5.png)

上海不同朝向单位面积租金分布情况：

![上海不同朝向单位面积租金分布情况](img/%E4%B8%8A%E6%B5%B7%E4%B8%8D%E5%90%8C%E6%9C%9D%E5%90%91%E5%8D%95%E4%BD%8D%E9%9D%A2%E7%A7%AF%E7%A7%9F%E9%87%91%E5%88%86%E5%B8%83%E6%83%85%E5%86%B5.png)

广州不同朝向单位面积租金分布情况：

![广州不同朝向单位面积租金分布情况](img/%E5%B9%BF%E5%B7%9E%E4%B8%8D%E5%90%8C%E6%9C%9D%E5%90%91%E5%8D%95%E4%BD%8D%E9%9D%A2%E7%A7%AF%E7%A7%9F%E9%87%91%E5%88%86%E5%B8%83%E6%83%85%E5%86%B5.png)

深圳不同朝向单位面积租金分布情况：

![深圳不同朝向单位面积租金分布情况](img/%E6%B7%B1%E5%9C%B3%E4%B8%8D%E5%90%8C%E6%9C%9D%E5%90%91%E5%8D%95%E4%BD%8D%E9%9D%A2%E7%A7%AF%E7%A7%9F%E9%87%91%E5%88%86%E5%B8%83%E6%83%85%E5%86%B5.png)

泉州不同朝向单位面积租金分布情况：

![泉州深圳不同朝向单位面积租金分布情况](img/%E6%B3%89%E5%B7%9E%E4%B8%8D%E5%90%8C%E6%9C%9D%E5%90%91%E5%8D%95%E4%BD%8D%E9%9D%A2%E7%A7%AF%E7%A7%9F%E9%87%91%E5%88%86%E5%B8%83%E6%83%85%E5%86%B5.png)

由于单位面积租金非常大的数据较少，为了跟好的观察分布情况，限制了 y 轴的显式范围，图中并不包含所有数据。

以下仅考虑东西南北四个方向，例如东南这种对角方向将同时作为东和南方向看待，由图中数据可以看出：

* 不论哪个城市，南朝向的房屋数量最多，且在单位面积租金较大的区域内，南朝向的房屋数量依旧是最密集的。原因是中国位于南半球，对于中国地区而言，太阳始终是从南方向向北方向照射，南方向的房屋具备更好地光照条件，因此相比于其他方向会更贵
* 对于泉州、深圳、广州而言，西方向的房屋数量最少，且在单位面积租金较大的区域内，西朝向的房屋数量最稀疏。原因来自于，泉州、深圳、广州三个城市里赤道较近，傍晚的太阳将以非常大的入射角从西偏南的方向射入房屋内，这种光照由于入射角非常大，太阳光可以直接射入屋内，一般称之为“西晒”。因此，西朝向的房屋相比于其他方向会更便宜
* 而对于北京和上海，由于地理位置偏北，“西晒”不明显。因此，东西方向的差距并不明显，两个方向的房屋都比南北方向的房屋便宜

### 分析并展示各个城市的人均 GDP 和单位面积租金分布的关系

上网搜索 5 个城市的人均 GDP，并采用`violinplot`展示其与单位面积租金分布的关系：

![人均GDP和单位面积租金分布的关系](img/%E4%BA%BA%E5%9D%87GDP%E5%92%8C%E5%8D%95%E4%BD%8D%E9%9D%A2%E7%A7%AF%E7%A7%9F%E9%87%91%E5%88%86%E5%B8%83%E7%9A%84%E5%85%B3%E7%B3%BB.png)

为了更好判断租房的性价比，图中用实心点标出了每个城市单位面积租金的中位数，并为了更好地展示斜率将其与点(0, 0)连接。

连线的斜率表示单位面积租金的中位数与人均 GDP 的比值，斜率越小表示相同人均 GDP 的情况下，可以支付的租房面积越大，性价比也越高。由图中可以看出，相对而言，泉州的租房性价比最高。


### 分析并展示各个城市的全口径平均工资和单位面积租金分布的关系

上网搜索 5 个城市的全口径平均工资，并采用`violinplot`展示其与单位面积租金分布的关系：

![平均工资和单位面积租金分布的关系](img/%E5%B9%B3%E5%9D%87%E5%B7%A5%E8%B5%84%E5%92%8C%E5%8D%95%E4%BD%8D%E9%9D%A2%E7%A7%AF%E7%A7%9F%E9%87%91%E5%88%86%E5%B8%83%E7%9A%84%E5%85%B3%E7%B3%BB.png)

为了更好判断租房的负担，图中用实心点标出了每个城市单位面积租金的中位数，并为了更好地展示斜率将其与点(0, 0)连接。

连线的斜率表示单位面积租金的中位数与平均工资的比值，斜率越大表示相同平均工资的情况下，需要支付的单位面积租金越大，负担也越重。由图中可以看出，相对而言，北京的租房负担最大。