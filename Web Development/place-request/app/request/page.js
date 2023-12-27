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
import { useSession } from 'next-auth/react';
import RequestCard from '@/components/place-request/requestCard';
import CreateWelcome from '@/components/place-request/createWelcome';
import WelcomeCardPool from '@/components/place-request/welcomeCardPool';

export default function Page() {
    const searchParams = useSearchParams();
    const requestId = searchParams.get('request_id');
    const { data: session } = useSession();

    const RequestCardMemo = React.memo(RequestCard);

    return (
        <>
            <Suspense fallback={<Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />}>
                <RequestCardMemo requestId={requestId} />

                <Box h='10' />

                {session ? (
                    <CreateWelcome requestId={requestId} userId={session.user.userId} />
                ) : (
                    <Card align='left' w='800px'>
                        <Alert status='error'>
                            <AlertIcon />
                            <AlertTitle>登录后可回复</AlertTitle>
                        </Alert >
                    </Card >
                )
                }

                <Box h='10' />
                <WelcomeCardPool requestId={requestId} />
            </Suspense>
        </>
    );
}