'use client'

import {
    Box,
    Flex,
    Card,
    CardBody,
    CardHeader,
    Heading,
    Grid,
    GridItem,
    Text,
    Input,
    Textarea,
    Spinner,

} from '@chakra-ui/react';
import { useState } from 'react';
import { useEffect } from 'react';
import { useSession } from 'next-auth/react';

import config from '@/app/config';
import UserIcon from '../shared/userIcon';
import { useSearchParams } from 'next/navigation';


export default function UserCard() {
    const { data: session, status } = useSession();
    const userId = useSearchParams().get('user_id');

    const [isLoading, setIsLoading] = useState(true);
    const [user, setUser] = useState({
        id: '',
        username: '',
        realName: '',
        identityType: '',
        identityNumber: '',
        phoneNumber: '',
        country: '',
        province: '',
        city: '',
        description: '',
    });

    useEffect(() => {
        let isFetched = false;

        if (status != 'loading' && !isFetched) {
            fetch(`${config.serverIp}/users/${userId}`, {
                method: 'GET',
                headers: {
                    'Authorization': `Bearer ${session.accessToken}`
                },
            }).then((res) => res.json())
                .then((user) => {
                    if (user?.username != undefined || user?.username != null) {
                        setUser(user);
                        setIsLoading(false);
                        isFetched = true;
                    }
                })
        }
    }, [status, userId])

    if (isLoading) {
        return (<>
            <Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />
        </>);
    }

    return (
        <Card align='center'>
            <CardHeader>
                <Flex align='center'>
                    <UserIcon boxSize='60px' />
                    <Box w='20px' />
                    <Heading fontSize=' 40px' >{user.username}</Heading>
                </Flex>
            </CardHeader>

            <CardBody>
                <Grid templateRows='repeat(8, 1fr)' templateColumns='repeat(6, 1fr)' gap={1}>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>ID</Text>
                        <Input variant='filled' isReadOnly='true' value={user.id} />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>用户</Text>
                        <Input variant='filled' isReadOnly='true' value={user.username} />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>真实姓名</Text>
                        <Input variant='filled' isReadOnly='true' value={user.realName} />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>证件类型</Text>
                        <Input variant='filled' isReadOnly='true' value={user.identityType} />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>证件号</Text>
                        <Input variant='filled' isReadOnly='true' value={user.identityNumber} />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>电话号码</Text>
                        <Input variant='filled' isReadOnly='true' value={user.phoneNumber}
                        />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={2} p='4'>
                        <Text>国家</Text>
                        <Input variant='filled' isReadOnly='true' value={user.country} />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={2} p='4'>
                        <Text>省份</Text>
                        <Input variant='filled' isReadOnly='true' value={user.province} />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={2} p='4'>
                        <Text>市</Text>
                        <Input variant='filled' isReadOnly='true' value={user.city} />
                    </GridItem>
                    <GridItem rowSpan={3} colSpan={6} p='4'>
                        <Text>个人简介</Text>
                        <Textarea variant='filled' isReadOnly='true' value={user.description}
                        />
                    </GridItem>
                </Grid>
            </CardBody>
        </Card >
    );
}