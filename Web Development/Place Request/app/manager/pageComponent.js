'use client'
import {
    Box,
    Flex,
    Stack,
    Card,
    Alert,
    AlertIcon,
    AlertTitle,
    Spinner,
} from "@chakra-ui/react";

import { useSearchParams } from "next/navigation";
import { useEffect } from "react";
import { useState } from "react";
import { useSession } from "next-auth/react";

import Filter from "@/components/shared/filter";
import config from '@/app/config';
import ManagerFilter from "./managerFilter";
import StatisticLineChart from "@/components/manager/statisticLineChart";
import StatisticTable from "@/components/manager/statisticTable"; 

export default function PageComponent() {
    const searchParams = useSearchParams();
    const { data: session, status } = useSession();

    const [isFilterSubmit, setIsFilterSubmit] = useState(false);
    const [monthDeals, setMonthDeals] = useState({});
    const [monthBrokerage, setMonthBrokerage] = useState({});
    const [deals, setDeals] = useState({});
    const [brokerage, setBrokerage] = useState({});
    const [isLoading, setIsLoading] = useState(true);


    const currentDate = new Date();
    let newSearchParams = new URLSearchParams(searchParams.toString());
    if (!searchParams.has('end_month')) {
        const currentYear = currentDate.getFullYear();
        const currentMonth = currentDate.getMonth() + 1; // getMonth() 返回的月份是从0开始的
        newSearchParams.set('end_month', `${currentYear}-${currentMonth.toString().padStart(2, '0')}`);
    }
    if (!searchParams.has('start_month')) {

        currentDate.setMonth(currentDate.getMonth() - 3);
        const pastYear = currentDate.getFullYear();
        const pastMonth = currentDate.getMonth() + 1;
        newSearchParams.set('start_month', `${pastYear}-${pastMonth.toString().padStart(2, '0')}`);
    }

    const startMonth = new Date(newSearchParams.get('start_month'));
    const endMonth = new Date(newSearchParams.get('end_month'));
    let current = new Date(startMonth);
    const months = [];

    while (current <= endMonth) {
        months.push(`${current.getFullYear()}-${String(current.getMonth() + 1).padStart(2, '0')}`);
        current.setMonth(current.getMonth() + 1);
    }

    useEffect(() => {
        let isFetched = false;

        if (status != 'loading' && !isFetched) {
            if (session && session.user.userType == 'Admin') {
                fetch(`${config.serverIp}/deals/statistics?${newSearchParams.toString()}`, {
                method: 'GET',
                headers: {
                    'Authorization': `Bearer ${session.accessToken}`
                },
            }).then((res) => res.json())
                    .then((datas) => {
                        let newDeals = {};
                        let newBrokerage = {};
                        let newMonthDeals = {};
                        let newMonthBrokerage = {};
                        if (datas != undefined || datas != null) {
                            datas.map((data) => {
                                if (!newMonthDeals[data.yearMonth]) {
                                    newMonthDeals[data.yearMonth] = parseInt(data.totalDeals);
                                    newMonthBrokerage[data.yearMonth] = parseInt(data.totalBrokerage);
                                }
                                else {
                                    newMonthDeals[data.yearMonth] += parseInt(data.totalDeals);
                                    newMonthBrokerage[data.yearMonth] += parseInt(data.totalBrokerage);
                                }
                                newDeals[data.yearMonth] = newDeals[data.yearMonth] || {};
                                newDeals[data.yearMonth][data.destinationType] = parseInt(data.totalDeals);
                                newBrokerage[data.yearMonth] = newBrokerage[data.yearMonth] || {};
                                newBrokerage[data.yearMonth][data.destinationType] = parseInt(data.totalBrokerage);
                            })
                        }
                        setDeals(newDeals);
                        setBrokerage(newBrokerage);
                        setMonthDeals(newMonthDeals);
                        setMonthBrokerage(newMonthBrokerage);
                        setIsLoading(false);
                    }
                    )
            }
        }
        else {
            setIsLoading(false);
            isFetched = true;
        }
    }, [status, searchParams])

    if (isLoading) {
        return (<>
            <Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />
        </>);
    }


    return (
        <>
            {session && session.user.userType == 'Admin' ? (
                <>
            <Flex
                position="fixed"
                top="30%"
                left="10%"
                transform="translate(-30%, -15%)"
                w='auto'
                h='60%'
                zIndex="10"
            >
                <Filter setIsSubmit={setIsFilterSubmit}>
                    <ManagerFilter isSubmit={isFilterSubmit} setIsSubmit={setIsFilterSubmit} />
                </Filter>
            </Flex>
                    <Stack justify="center" align="center" w='full'>
                        <StatisticLineChart months={months} datas={monthDeals} stroke='#8884d8' chartName='月-成交单数折线图' />
                        <Box h='6' />
                        <StatisticLineChart months={months} datas={monthBrokerage} stroke='#82ca9d' chartName='月-中介费折线图' />
                        <Box h='6' />
                        <StatisticTable months={months} data={deals} title='月-去处类型成交单数统计' />
                        <Box h='6' />
                        <StatisticTable months={months} data={brokerage} title='月-去处类型中介费统计' />
            </Stack>
                </>
            ) : (
                <>
                    <Card>
                        <Alert status='error'>
                            <AlertIcon />
                            <AlertTitle>管理员界面</AlertTitle>
                        </Alert >
                    </Card >
                </>
            )}
        </>
    );
}