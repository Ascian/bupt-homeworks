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
import WelcomeCardPreview from './welcomeCardPreview';
import config from '@/app/config';

export default async function WelcomeCardPreviewPool() {
    const { data: session } = useSession();
    const [page, setPage] = useState(1);

    useEffect(() => {
        fetch(`${config.serverIp}/offers/mine?page=${page}&pageSize=10`, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${session.accessToken}`
            },
        }).then((res) => res.json())
            .then((response) => {
                if (response?.pageNum) {
                    setMaxPage(response.pageNum);
                    setWelcomes(response.data);
                    setIsLoading(false);
                }
            })
    }, [session, page])


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