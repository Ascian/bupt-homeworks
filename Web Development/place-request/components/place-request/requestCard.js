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

export default async function RequestCard({ requestId }) {
    const res = await fetch(`${config.serverIp}/seekers/${requestId}`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
        },
    })
    const request = await res.json()
    if (!res.ok || !request) {
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

    const createTime = new Date(request.createTime).toLocaleDateString()

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
                    <Text fontSize='md' color='grey' >{`${createTime}    ${request.region}`}</Text>
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