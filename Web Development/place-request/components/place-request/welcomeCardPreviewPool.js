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
import { useSearchParams } from 'next/navigation';
import Pagination from "../shared/pagination";
import WelcomeCardPreview from './welcomeCardPreview';
import config from '@/app/config';

export default function WelcomeCardPreviewPool() {
    const { data: session, status } = useSession();
    const page = useSearchParams().get('page') || 1;
    const [maxPage, setMaxPage] = useState(0);
    const [welcomes, setWelcomes] = useState([]);
    const [isLoading, setIsLoading] = useState(true);

    useEffect(() => {
        let isFetched = false;
        if (status != 'loading' && !isFetched) {
        fetch(`${config.serverIp}/offers/mine?page=${page}&pageSize=10`, {
            method: 'GET',
            headers: {
                'Authorization': `Bearer ${session.accessToken}`
            },
        }).then((res) => res.json())
            .then((response) => {
                let data = null;
                if (response?.pageNum != undefined || response?.pageNum != null) {
                    setMaxPage(response.pageNum);
                    data = response.data;
                    setIsLoading(false);
                }
                Promise.all(data.map((welcome) =>
                    fetch(`${config.serverIp}/seekers/${welcome.seekerId}`, {
                        method: 'GET',

                    })
                        .then(res => res.json())
                        .then(request => {
                            if (request?.seekerId) {
                                return { ...welcome, requestTitle: request.seekerTitle };
                            } else {
                                return welcome;
                            }
                        })
                        .catch(() => {
                            return welcome;
                        })
                )).then(welcomes => {
                    setWelcomes(welcomes);
                    setIsLoading(false); // 所有请求完成后再改变加载状态
                }); 
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
                        <Pagination maxPage={maxPage} />
                </>
            )}
        </>
    );
}