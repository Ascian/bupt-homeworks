'use client'

import {
    HStack,
    Flex,
    Box,
    Card,
    CardBody,
    CardHeader,
    Heading,
    Grid,
    GridItem,
    Container,
    Text,
    Input,
    Image,
    CardFooter,
    Button,
    Menu,
    MenuButton,
    MenuList,
    MenuItem,
    Textarea,
    useToast,
    Spinner,
} from '@chakra-ui/react';
import { Suspense } from 'react';
import { useState, } from 'react';
import PublicRequestCardPool from '@/components/place-request/publicRequestCardPool';

export default function Page() {
    const [page, setPage] = useState(1);
    const maxPage = 2;

    return (
        <>
            <Suspense fallback={<Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />}>
                <PublicRequestCardPool page={page} />

                <Box h='10' />
                <Card bg="transparent" boxShadow="none" align='center' justify='center'>
                    <Text align='center' fontSize='lg'>第 {page} 页</Text>
                </Card>
                <Box h='6' />

                <Flex align='center' justify='center'>
                    <>
                        {page === 1 ? <></> : (
                            <>
                                <Button onClick={() => setPage(page - 1)} >{'< '}上一页</Button>
                                <Box w='8' />
                            </>
                        )}
                    </>
                    <>
                        {page === maxPage ? <></> : (
                            <>
                                <Button onClick={() => setPage(page + 1)}>下一页{' >'}</Button>
                                <Box w='8' />
                            </>
                        )}
                    </>
                </Flex>
            </Suspense>
        </>
    );
}