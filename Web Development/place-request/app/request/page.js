'use client'

import {
    Box,
    Spinner,
    Card,
    Alert,
    AlertIcon,
    AlertTitle,

} from '@chakra-ui/react';
import React, { Suspense } from 'react';
import { useSearchParams } from 'next/navigation';
import RequestCard from '@/components/place-request/requestCard';
import WelcomeCardPool from '@/components/place-request/welcomeCardPool';
import DynamicPart from './dynamicPart';

export default function Page() {
    const searchParams = useSearchParams();
    const requestId = searchParams.get('request_id');

    return (
        <>
            <Suspense fallback={<Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />}>
                <RequestCard requestId={requestId} />

                <Box h='10' />

                <DynamicPart requestId={requestId} />

            </Suspense>
        </>
    );
}