'use client'

import { Spinner } from '@chakra-ui/react';
import { Suspense } from 'react';
import { useSearchParams } from 'next/navigation';
import RequestCard from '@/components/place-request/requestCard';

export default function Page() {
    const searchParams = useSearchParams();
    console.log(searchParams);

    return (
        <>
            <Suspense fallback={<Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />}>
                <RequestCard requestId={searchParams.get('requestId')} />
            </Suspense>
        </>
    );
}