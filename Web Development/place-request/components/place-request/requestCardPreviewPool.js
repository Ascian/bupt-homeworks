'use client'

import {
    Box,
    Spinner,
    Card,
    Alert,
    AlertIcon,
    AlertTitle,
    Flex,
    Stack,

} from '@chakra-ui/react';
import { useEffect, useState } from 'react';
import { useSession } from 'next-auth/react';
import { useSearchParams } from 'next/navigation';

import Pagination from "../shared/pagination";
import RequestCardPreview from './requestCardPreview';
import config from '@/app/config';
import Filter from '../shared/filter';
import RequestFilter from '@/app/user/requests/requestFilter'

export default function RequestCardPreviewPool() {
    const { data: session, status } = useSession();
    const page = useSearchParams().get('page') || 1;
    const [maxPage, setMaxPage] = useState(0);
    const [requests, setRequests] = useState([]);
    const [isLoading, setIsLoading] = useState(true);
    const [isFilterSubmit, setIsFilterSubmit] = useState(false);

    const searchParams = useSearchParams()

    useEffect(() => {
        let isFetched = false;

        if (status != 'loading' && !isFetched) {
            fetch(`${config.serverIp}/seekers/mine?pageSize=10&${searchParams.toString()}`, {
                method: 'GET',
                headers: {
                    "Authorization": `Bearer ${session.accessToken}`
                }

            }).then((res) => res.json())
                .then((response) => {
                    if (response?.pageNum != undefined || response?.pageNum != null) {
                        setMaxPage(response.pageNum);
                        setRequests(response.data);
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
                left="10%"
                transform="translate(-30%, -15%)"
                w='auto'
                h='60%'
                zIndex="10"
            >
                <Filter setIsSubmit={setIsFilterSubmit}>
                    <RequestFilter isSubmit={isFilterSubmit} setIsSubmit={setIsFilterSubmit} />
                </Filter>
            </Flex>
            <Stack justify="center" align="center" w='full'>
            {maxPage == 0 ? (
                <Card>
                    <Alert status="warning">
                        <AlertIcon />
                            <AlertTitle >您还没有此类请求！</AlertTitle>
                    </Alert>
                </Card>
            ) : (
                <>
                    {
                        requests.map((request) => (
                            <>
                                <RequestCardPreview request={request} key={request.seekerId} />
                                <Box h='2' />
                            </>
                        ))
                    }
                    </>
                )}
            </Stack>
            < Box h='10' />
            <Pagination maxPage={maxPage} />
        </>
    )
}