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
import config from '@/app/config'
import { useState, useEffect } from 'react';
import { useSearchParams, useRouter, usePathname } from 'next/navigation';

export default function HomeFilter({ isSubmit, setIsSubmit }) {
    const [destinationType, setDestinationType] = useState(undefined);

    const router = useRouter();
    const pathname = usePathname();
    const searchParams = useSearchParams();

    useEffect(() => {
        if (isSubmit) {
            let newSearchParams = new URLSearchParams(searchParams);
            if (destinationType) {
                newSearchParams.set('destination_type_list', destinationType);
            }
            else {
                newSearchParams.delete('destination_type_list');
            }
            router.push(`${pathname}?${newSearchParams.toString()}`);
            setIsSubmit(false);
        }

    }, [isSubmit]);

    return (
        <>
            <Heading fontSize='md'>类型</Heading>
            <Box h='2' />
            <Menu >
                <MenuButton w='150px' as={Button} rightIcon={<ChevronDownIcon />} >
                    <Flex justify='left'>
                        {destinationType ? destinationType : '全部'}
                    </Flex>
                </MenuButton>
                <MenuList maxHeight="300px" overflowY="auto">
                    <MenuItem onClick={() => setDestinationType(undefined)}>全部</MenuItem>
                    {config.destinationTypes.map((type) => (
                        <MenuItem onClick={() => setDestinationType(type)}>{type}</MenuItem>
                    ))}
                </MenuList>
            </Menu>
        </>
    );
}   