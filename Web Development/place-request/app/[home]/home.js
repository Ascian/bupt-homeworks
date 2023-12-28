'use client'

import {
    Card,
    Spinner,
    Alert,
    AlertIcon,
    AlertTitle,
    Flex,

} from '@chakra-ui/react';
import { useEffect, useState } from 'react';
import { useSession } from 'next-auth/react';

import LargeRequestCardPreviewPool from '@/components/place-request/largeRequestCardPreviewPool';
import Pagination from '@/components/shared/pagination';
import Filter from '@/components/shared/filter';
import HomeFilter from './homeFilter';
import config from '@/app/config';
import { useSearchParams } from 'next/navigation';

export default async function Home() {
    const { data: session, status } = useSession();

    const [requests, setRequests] = useState([]);
    const [maxPage, setMaxPage] = useState(0);
    const [isLoading, setIsLoading] = useState(true);
    const [isFilterSubmit, setIsFilterSubmit] = useState(false);

    const searchParams = useSearchParams()


    useEffect(() => {
        let isFetched = false;

        if (status != 'loading' && !isFetched) {

            fetch(`${config.serverIp}/seekers?page_size=10${session ? `&user_region=${session.user.city}` : ''}&status_list=Active${searchParams ? `&${searchParams.toString()}` : ''}`, {
                method: 'GET',
            }).then((res) => res.json())
                .then((response) => {
                    if (response?.pageNum != undefined || response?.pageNum != null) {
                        setRequests(response.data);
                        setMaxPage(response.pageNum);
                        setIsLoading(false);
                        isFetched = true;
                    }
                })
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
            <Flex
                position="fixed"
                top="30%"
                left="15%"
                transform="translate(-30%, -15%)"
                w='auto'
                h='60%'
                zIndex="10"
            >
                <Filter setIsSubmit={setIsFilterSubmit}>
                    <HomeFilter isSubmit={isFilterSubmit} setIsSubmit={setIsFilterSubmit} />
                </Filter>
            </Flex>
            <Flex justify="center" align="center" w='full'>
                {maxPage == 0 ? (
                    <Card>
                        <Alert status="warning">
                            <AlertIcon />
                            <AlertTitle >您所在地区不存在此类请求！</AlertTitle>
                        </Alert>
                    </Card>
                ) : (
                    <>
                            <LargeRequestCardPreviewPool requests={requests} />

                    </>
                )}
            </Flex>
            <Pagination maxPage={maxPage} />
        </>
    );
}