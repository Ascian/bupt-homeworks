'use client'

import {
    Box,
    Heading,
    Input,
} from '@chakra-ui/react';
import { useState, useEffect } from 'react';
import { useSearchParams, useRouter, usePathname } from 'next/navigation';
import { useToast } from '@chakra-ui/react';

export default function ManagerFilter({ isSubmit, setIsSubmit }) {
    const [startMonth, setStartMonth] = useState('');
    const [endMonth, setEndMonth] = useState('');
    const [region, setRegion] = useState('');

    const router = useRouter();
    const pathname = usePathname();
    const searchParams = useSearchParams();
    const toast = useToast();

    useEffect(() => {
        if (isSubmit) {
            if (startMonth && endMonth && startMonth > endMonth) {
                toast({
                    title: '年月筛选不合法',
                    description: '起始年月不能晚于终止年月',
                    status: 'error',
                    duration: 9000,
                    isClosable: true,
                })

                setIsSubmit(false);
                return;
            }
            if (region.length > 20) {
                toast({
                    title: '地区筛选不合法',
                    description: '地区筛选长度不能超过20',
                    status: 'error',
                    duration: 9000,
                    isClosable: true,
                })

                setIsSubmit(false);
                return;
            }
            let newSearchParams = new URLSearchParams(searchParams);
            if (startMonth) {
                newSearchParams.set('start_month', startMonth);
            }
            else {
                newSearchParams.delete('start_month');
            }
            if (endMonth) {
                newSearchParams.set('end_month', endMonth);
            }
            else {
                newSearchParams.delete('end_month');
            }
            if (region) {
                newSearchParams.set('region', region);
            }
            else {
                newSearchParams.delete('region');
            }
            router.push(`${pathname}?${newSearchParams.toString()}`);
            setIsSubmit(false);
        }

    }, [isSubmit]);

    return (
        <>
            <Heading fontSize='md'>起始年月</Heading>
            <Box h='2' />
            <Input
                value={startMonth}
                onChange={(e) => setStartMonth(e.target.value)}
                type="month"
            />
            <Box h='4' />
            <Heading fontSize='md'>终止年月</Heading>
            <Box h='2' />
            <Input
                value={endMonth}
                onChange={(e) => setEndMonth(e.target.value)}
                type="month"
            />
            <Box h='4' />
            <Heading fontSize='md'>地区</Heading>
            <Box h='2' />
            <Input
                value={region}
                onChange={(e) => setRegion(e.target.value)}
            />
        </>
    );
}   