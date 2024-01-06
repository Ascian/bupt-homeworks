
import {
    Spinner,
} from '@chakra-ui/react';
import React, { Suspense } from 'react';
import PageComponent from './pageComponent';

export default function Page() {

    return (
        <>
            <Suspense fallback={<Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />}>
                <PageComponent />

            </Suspense>
        </>
    );
}