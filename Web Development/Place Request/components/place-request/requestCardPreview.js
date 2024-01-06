'use client'

import {
    CardBody,
    Card,
    Text,
    Divider,
    Flex,
    Box,
    Link,
    Heading,


} from "@chakra-ui/react";

export default function RequestCardPreview({ request }) {

    const updateTime = new Date(request.updateTime).toLocaleDateString();
    const seekerExpiryDate = new Date(request.seekerExpiryDate).toLocaleDateString();

    const statusColor = {
        'Active': 'green',
        'Completed': 'blue',
        'Expired': 'grey',
        'Cancelled': 'grey',
    }
    const status = {
        'Active': '进行中',
        'Completed': '已完成',
        'Expired': '已过期',
        'Cancelled': '已取消',
    }

    return (
        <Link href={`/request?request_id=${request.seekerId}`}
            style={{
                textDecoration: 'none'
            }} >
            <Card w='750px' h='auto'>
                <CardBody>
                    <Heading fontSize='lg' maxW='700px' noOfLines={[1]} >{request.seekerTitle}</Heading>
                    <Divider orientation='horizontal' w='700px' />
                    <Box h='2' />
                    <Flex justify="flex-start" align='center' w='700px' >
                        <Box width='520px'>
                            <Flex w='520px' >
                                <Text fontSize='md' w='50px'>请求：</Text>

                                <Box w="1" />
                                <Text fontSize='md' noOfLines={[1, 2, 3]}>{request.seekerDescription}</Text>
                            </Flex>
                            <Box h='2' />
                            <Flex w='520px' >
                                <Text color='grey' width='50%'>最近更新于{updateTime}</Text>
                                <Text color='grey'>截止于{seekerExpiryDate}</Text>
                            </Flex>
                        </Box>
                        <Box w='20px' />
                        <Box w='80px'>
                            <Flex w='80px'>
                                <Heading fontSize='lg' w='20px' >￥</Heading>
                                <Heading fontSize='lg' maxW='60px'>{request.maxExpectedPrice}</Heading>
                            </Flex>
                        </Box>
                        <Box w='20px' />
                        <Box w='70px'>
                            <Heading fontSize='lg' color={statusColor[request.status]}
                            >{status[request.status]}</Heading>
                        </Box>
                    </Flex>
                </CardBody>
            </Card>
        </Link >
    );
}
