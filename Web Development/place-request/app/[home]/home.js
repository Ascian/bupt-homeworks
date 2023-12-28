'use client'

import {
    Box,
    Card,
    Text,
    Spinner,
    Alert,
    AlertIcon,
    AlertTitle,
} from '@chakra-ui/react';
import { useEffect, useState } from 'react';
import { useSession } from 'next-auth/react';
import LargeRequestCardPreviewPool from '@/components/place-request/largeRequestCardPreviewPool';
import Pagination from '@/components/shared/pagination';
import config from '@/app/config';

export default function Home() {
    const [page, setPage] = useState(1);
    const [maxPage, setMaxPage] = useState(0);
    const [isLoading, setIsLoading] = useState(true);
    const { data: session, status } = useSession();

    useEffect(() => {
        let isFetched = false;

        if (status != 'loading' && !isFetched) {
        fetch(`${config.serverIp}/seekers${session ? `?user_region=${session.user.city}` : ''}`, {
            method: 'GET',
            headers: {
                "Content-Type": "application/json",
            },
        })
            .then((res) => res.json())
            .then((response) => {
                if (response?.pageNum != undefined || response?.pageNum != null) {
                    setMaxPage(response.pageNum);
                    setIsLoading(false);
                    isFetched = true;
                }
            })
        }
    }, [status])

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
            {maxPage == 0 ? (
                <Card>
                    <Alert status="warning">
                        <AlertIcon />
                        <AlertTitle >您所在地区不存在求助求助！</AlertTitle>
                    </Alert>
                </Card>
            ) : (
                <>
            <LargeRequestCardPreviewPool page={page} />

            <Box h='10' />
            <Card bg="transparent" boxShadow="none" align='center' justify='center'>
                <Text align='center' fontSize='lg'>第 {page} 页</Text>
            </Card>
            <Box h='6' />

            <Pagination setPage={setPage} page={page} maxPage={maxPage} />
                </>
            )}
        </>
    );
}