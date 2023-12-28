'use client'

import { Box, Button, Flex, Input, Text, Card } from '@chakra-ui/react';
import { useState } from 'react';
import { useSearchParams } from 'next/navigation'
import { useToast } from '@chakra-ui/toast';

export default function Pagination({ maxPage }) {
    const [newPage, setNewPage] = useState(1);

    const searchParams = useSearchParams();
    const page = searchParams.get('page') || 1;

    const setPage = (newPage) => {
        const pathname = window.location.pathname;
        window.location.href = `${pathname}?page=${newPage}`;
    }

    const toast = useToast();

    return (
        <>
            <Box h='10' />
            <Card bg="transparent" boxShadow="none" align='center' justify='center'>
                <Text align='center' fontSize='lg'>第 {page} 页</Text>
            </Card>
            <Box h='6' />
            <Flex align='center' justify='center'>
                <>
                    {page == 1 ? <></> : (
                        <>
                            <Button onClick={() => setPage(parseInt(page) - 1)} >{'< '}上一页</Button>
                            <Box w='8' />
                        </>
                    )}
                </>
                <>
                    {page == maxPage ? <></> : (
                        <>
                            <Button onClick={() => setPage(parseInt(page) + 1)}>下一页{' >'}</Button>
                            <Box w='8' />
                        </>
                    )}
                </>
                <Flex align='center' justify='center'>
                    <Button onClick={() => {
                        if (newPage > maxPage || newPage < 1) {
                            toast({
                                title: '页码超出范围',
                                description: `请输入 1 - ${maxPage} 之间的页码`,
                                status: 'error',
                                duration: 3000,
                                isClosable: true,
                            })
                            return;
                        }
                        setPage(newPage)
                    }}>跳转</Button>
                    <Card bg="transparent" boxShadow="none" align='center' justify='center'>
                        <Text align='center' >至</Text>
                    </Card>
                    <Input variant='flushed' bgColor='white' w='8' textAlign="center" value={newPage} onChange={(e) => setNewPage(e.target.value)} />
                    <Card bg="transparent" boxShadow="none" align='center' justify='center'>
                        <Text align='center' >页</Text>
                    </Card>
                </Flex>
            </Flex>
        </>
    );
}