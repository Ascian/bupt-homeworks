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

export default function WelcomeCardPreview({ welcome }) {
    const updateTime = new Date(welcome.updateTime).toLocaleDateString();

    const statusColor = {
        'Active': 'green',
        'Accepted': 'blue',
        'Declined': 'red',
        'Cancelled': 'grey',
        'Expired': 'grey'
    }

    const status = {
        'Active': '进行中',
        'Accepted': '已接受',
        'Declined': '已拒绝',
        'Cancelled': '已取消',
        'Expired': '已过期'
    }

    return (
        <Link href={`/request?request_id=${welcome.seekerId}`}
            style={{
                textDecoration: 'none'
            }} >
            <Card w='750px' h='auto'>
                <CardBody>
                    <Heading fontSize='lg' maxW='700px' noOfLines={[1]}  >{welcome.requestTitle}</Heading>
                    <Divider orientation='horizontal' w='700px' />
                    <Box h='2' />
                    <Flex justify="flex-start" align='center' w='700px' >
                        <Box width='590px'>
                            <Flex w='590px' >
                                <Text fontSize='md' w='50px'>回复：</Text>

                                <Box w="1" />
                                <Text fontSize='md' noOfLines={[1, 2, 3]}>{welcome.offerDescription}</Text>
                            </Flex>
                            <Box h='2' />
                            <Text color='grey'>最近更新于{updateTime}</Text>
                        </Box>
                        <Box w='20px' />
                        <Box w='70px'>
                            <Heading fontSize='lg' color={statusColor[welcome.status]}
                            >{status[welcome.status]}</Heading>
                        </Box>
                    </Flex>
                </CardBody>
            </Card>
        </Link >
    );
}
