'use client'

import {
    Box,
    Heading,
    Input,
} from '@chakra-ui/react';
import { useState, useEffect } from 'react';
import { useSearchParams, useRouter, usePathname } from 'next/navigation';
import { useToast } from '@chakra-ui/react';

export default function UsersFilter({ isSubmit, setIsSubmit }) {
    const [userId, setUserId] = useState('');

    const router = useRouter();
    const pathname = usePathname();
    const searchParams = useSearchParams();
    const toast = useToast();

    useEffect(() => {
        if (isSubmit) {
            if (userId.length > 20) {
                toast({
                    title: '用户ID筛选不合法',
                    description: '用户ID筛选长度不能超过20',
                    status: 'error',
                    duration: 9000,
                    isClosable: true,
                })

                setIsSubmit(false);
                return;
            }

            let newSearchParams = new URLSearchParams(searchParams);
            if (userId) {
                newSearchParams.set('user_id', userId);
            }
            else {
                newSearchParams.delete('user_id');
            }
            router.push(`${pathname}?${newSearchParams.toString()}`);
            setIsSubmit(false);
        }

    }, [isSubmit]);

    return (
        <>
            <Heading fontSize='md'>用户ID</Heading>
            <Box h='2' />
            <Input
                value={userId}
                onChange={(e) => setUserId(e.target.value)}
            />
        </>
    );
}   