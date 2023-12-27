'use client'
import {
    Spinner,
} from '@chakra-ui/react';
import { Suspense } from 'react';
import { useSearchParams } from 'next/navigation';
import ModifyRequest from "@/components/place-request/modifyRequest";

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
                <ModifyRequest requestId={requestId} />
            </Suspense>
        </>
    );
}