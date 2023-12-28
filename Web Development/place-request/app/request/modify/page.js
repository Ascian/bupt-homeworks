
import {
    Spinner,
} from '@chakra-ui/react';
import { Suspense } from 'react';
import ModifyRequest from "@/components/place-request/modifyRequest";

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
                <ModifyRequest />
            </Suspense>
        </>
    );
}