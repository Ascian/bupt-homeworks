'use client'

import { useSearchParams } from "next/navigation";
import { useEffect } from "react";

export default function PageComponent() {
    const searchParams = useSearchParams();
    const { data: session, status } = useSession();

    const [yearMonth, setYearMonth] = useState('');
    const [totalDeals, setTotalDeals] = useState(0);
    const [totalBrokerage, setTotalBrokerage] = useState(0);


    let searchParamsString = searchParams.toString();
    if (searchParamsString == '') {
        const currentDate = new Date();
        const currentYear = currentDate.getFullYear();
        const currentMonth = currentDate.getMonth() + 1; // getMonth() 返回的月份是从0开始的

        console.log(`当前年月: ${currentYear}-${currentMonth.toString().padStart(2, '0')}`);

        currentDate.setMonth(currentDate.getMonth() - 3);
        const pastYear = currentDate.getFullYear();
        const pastMonth = currentDate.getMonth() + 1;

        console.log(`三个月前的年月: ${pastYear}-${pastMonth.toString().padStart(2, '0')}`);
    }

    useEffect(() => {
        let isFetched = false;

        if (status != 'loading' && !isFetched) {
            fetch(`${config.serverIp}/deals/statistics&${searchParams}`, {
                method: 'GET',
                headers: {
                    'Authorization': `Bearer ${session.accessToken}`
                },
            }).then((res) => res.json())
                .then((response) => {
                    if (response?.totalDeals != undefined || response?.totalDeals != null) {
                        setYearMonth(response.yearMonth);
                        setTotalDeals(response.totalDeals);
                        setTotalBrokerage(response.totalBrokerage);
                        setIsLoading(false);
                        isFetched = true;
                    }
                })
        }
    }, [status, searchParams])


    return (
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
                <StatisticLineChart />
                <Box h='4' />
                <StatisticTable />
            </Stack>
        </>
    );
}