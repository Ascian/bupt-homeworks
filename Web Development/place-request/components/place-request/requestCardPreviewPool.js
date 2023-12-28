'use client'

import {
    Box,
    Spinner,
    Card,
    Alert,
    AlertIcon,
    AlertTitle,
} from '@chakra-ui/react';
import { useEffect, useState } from 'react';
import { useSession } from 'next-auth/react';
import Pagination from "../shared/pagination";
import RequestCardPreview from './requestCardPreview';
import config from '@/app/config';

export default function RequestCardPreviewPool() {
    const { data: session, status } = useSession();
    const [page, setPage] = useState(1);
    const [maxPage, setMaxPage] = useState(0);
    const [requests, setRequests] = useState([]);
    const [isLoading, setIsLoading] = useState(true);

    useEffect(() => {
        let isFetched = false;
        if (status != 'loading' && !isFetched) {
        fetch(`${config.serverIp}/seekers/mine?page=${page}&pageSize=10`, {
            method: 'GET',
            headers: {
                "Content-Type": "application/json",
                'Authorization': `Bearer ${session.accessToken}`
            },
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
    }, [status, page])

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
                        <AlertTitle >您还没有发布过求助！</AlertTitle>
                    </Alert>
                </Card>
            ) : (
                <>
                    {
                        requests.map((request) => (
                            <>
                                <RequestCardPreview request={request} />
                                <Box h='2' />
                            </>
                        ))
                    }
                    < Box h='10' />
                    <Pagination page={page} setPage={setPage} maxPage={maxPage} />
                </>
            )}
        </>
    )
}