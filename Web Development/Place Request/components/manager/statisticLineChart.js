'use client'

import {
    LineChart,
    Line,
    XAxis,
    YAxis,
    CartesianGrid,
    Tooltip,
    Legend,
    ResponsiveContainer,
} from 'recharts';
import { Card, Flex, Box, CardHeader, CardBody } from '@chakra-ui/react';


export default function StatisticLineChart({ months, datas, stroke, chartName }) {

    const data = months.map((month) => ({
        month: month,
        value: datas[month] ? datas[month] : 0,
    }));

    return (
        <>
            <Card w="700px" h="600px">
                <CardHeader fontSize='xl' fontWeight='bold' textAlign='center'>
                    {chartName}
                </CardHeader>
                <CardBody>
                    <Flex justifyContent="center" alignItems="center" w='full' h='500px' >
                        <ResponsiveContainer width="90%" height="90%">
                            <LineChart
                                width={500}
                                height={300}
                                data={data}
                                margin={{
                                    top: 5,
                                    right: 30,
                                    left: 20,
                                    bottom: 5,
                                }}
                            >
                                <CartesianGrid strokeDasharray="3 3" />
                                <XAxis dataKey="month" />
                                <YAxis />
                                <Tooltip />
                                <Legend />
                                <Line
                                    type="monotone"
                                    dataKey="value"
                                    stroke={stroke}
                                    activeDot={{ r: 8 }}
                                />
                            </LineChart>
                        </ResponsiveContainer>
                    </Flex>
                </CardBody>
            </Card>

        </>
    );
}