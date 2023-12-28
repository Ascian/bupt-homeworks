'use client'

import {
    Card,
    CardBody,
    Heading,
    Image,
    Text,
    Box,
    Flex,
    Spinner,
} from '@chakra-ui/react';
import config from '@/app/config'
import { useEffect, useState } from 'react';

export default async function RequestCard({ requestId }) {
    const [request, setRequest] = useState({});
    const [updateTime, setUpdateTime] = useState('');
    const [isLoading, setIsLoading] = useState(true);


    useEffect(() => {
        fetch(`${config.serverIp}/seekers/${requestId}`, {
            method: 'GET',
            headers: {
                "Content-Type": "application/json",
            },
        }).then((res) => res.json())
            .then((request) => {
                if (request) {
                    setRequest(request);
                    setUpdateTime(new Date(request.updateTime).toLocaleDateString());
                    setIsLoading(false);
                }
            })
    }, [requestId])

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
        <>
            <Card align='center' w='800px' h='auto'>

                <CardBody>
                    <Image w='600px' src="/picture.png" alt="picture" />
                    <Flex justify="flex-start" align='center' w='600px' >
                        <Flex justify="flex-start" align='center' w='600px' >
                            <Image borderRadius="full" boxSize="50px" src="/userIcon.png" alt="User Icon" />
                            <Box w="4" />
                            <Text fontSize='30px' >{request.username}</Text>
                        </Flex>
                    </Flex>
                    <Box h='4' />
                    <Heading fontSize='40px' maxW='600px' >{request.seekerTitle}</Heading>
                    <Box h='4' />
                    <Text fontSize='md' color='grey' >{`${updateTime}    ${request.userRegion}`}</Text>
                    <Box h='4' />
                    <Text fontSize='lg' maxW='600px' >{request.seekerDescription}</Text>
                    <Box h='4' />
                    <Text fontSize='md' color='grey' >{request.destinationType}</Text>
                    <Box h='4' />
                    <Flex justify="flex-end" align='center' w='600px' >
                        <Heading fontSize='20px' >最高单价：￥{request.maxExpectedPrice}</Heading>
                    </Flex>

                </CardBody>

            </Card >
        </>
    );
}