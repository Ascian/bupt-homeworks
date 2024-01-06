'use client'

import {
    Spinner,
    Card,
    Alert,
    AlertIcon,
    AlertTitle,
    Box,

} from "@chakra-ui/react";
import { Suspense } from "react";
import { useSession } from "next-auth/react";
import RequestCardPreviewPool from "@/components/place-request/requestCardPreviewPool";

export default function Page() {
    const { data: session } = useSession();

    return (
        <Box justifyContent='flex-start' alignContent='center' >
            <Suspense fallback={<Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />}>
                {session ? (
                    <RequestCardPreviewPool />
                ) : (
                    <Card align='center'>
                        <Alert status='error'>
                            <AlertIcon />
                            <AlertTitle>请先登录!</AlertTitle>
                        </Alert >
                    </Card >
                )}
            </Suspense>
        </Box>
    );
}