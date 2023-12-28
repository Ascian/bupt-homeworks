'use client'

import {
    Box,
    Button,
    Heading,
    Flex,
    Menu,
    MenuButton,
    MenuList,
    MenuItem,
} from '@chakra-ui/react';
import { ChevronDownIcon } from '@chakra-ui/icons'
import { useState, useEffect } from 'react';
import { useSearchParams, useRouter, usePathname } from 'next/navigation';

export default function OfferFilter({ isSubmit, setIsSubmit }) {
    const [statusType, setStatusType] = useState(undefined);

    const router = useRouter();
    const pathname = usePathname();
    const searchParams = useSearchParams();

    const statusTypes = [
        '进行中',
        '已接收',
        '已拒绝',
        '已取消',
        '已过期',
    ];

    const status = {
        '进行中': 'Active',
        '已接收': 'Accepted',
        '已拒绝': 'Declined',
        '已取消': 'Cancelled',
        '已过期': 'Expired',
    };

    useEffect(() => {
        if (isSubmit) {
            let newSearchParams = new URLSearchParams(searchParams);
            if (statusType) {
                newSearchParams.set('status_list', status[statusType]);
            }
            else {
                newSearchParams.delete('status_list');
            }
            router.push(`${pathname}?${newSearchParams.toString()}`);
            setIsSubmit(false);
        }

    }, [isSubmit]);

    return (
        <>
            <Heading fontSize='md'>状态</Heading>
            <Box h='2' />
            <Menu >
                <MenuButton w='150px' as={Button} rightIcon={<ChevronDownIcon />} >
                    <Flex justify='left'>
                        {statusType ? statusType : '全部'}
                    </Flex>
                </MenuButton>
                <MenuList maxHeight="300px" overflowY="auto">
                    <MenuItem onClick={() => setStatusType(undefined)}>全部</MenuItem>
                    {statusTypes.map((type) => (
                        <MenuItem onClick={() => setStatusType(type)}>{type}</MenuItem>
                    ))}
                </MenuList>
            </Menu>
        </>
    );
}   