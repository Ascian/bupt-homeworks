'use client'

import {
    Box,
    Spinner,
    Card,
    Alert,
    AlertIcon,
    AlertTitle,
} from '@chakra-ui/react';
import { useState } from 'react';
import { useSession } from 'next-auth/react';
import Pagination from "../shared/pagination";
import WelcomeCardPreview from './welcomeCardPreview';
import config from '@/app/config';

export default async function WelcomeCardPreviewPool() {
    const { data: session } = useSession();
    const [page, setPage] = useState(1);

    const res = await fetch(`${config.serverIp}/offers/mine?page=${page}&pageSize=10`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
            'Authorization': `Bearer ${session.accessToken}`
        },
    });
    const response = await res.json()
    if (!res.ok || response?.pageNum === null || response?.pageNum === undefined) {
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
    const maxPage = response.pageNum;
    const welcomes = response.data

    return (
        <>
            {maxPage == 0 ? (
                <Card>
                    <Alert status="warning">
                        <AlertIcon />
                        <AlertTitle >您还没有回复他人请求！</AlertTitle>
                    </Alert>
                </Card>
            ) : (
                <>
                    {
                        welcomes.map((welcome) => (
                            <>
                                <WelcomeCardPreview welcome={welcome} />
                                <Box h='2' />
                            </>
                        ))
                    }
                    < Box h='10' />
                    <Pagination page={page} setPage={setPage} maxPage={maxPage} />
                </>
            )}
        </>
    );
}