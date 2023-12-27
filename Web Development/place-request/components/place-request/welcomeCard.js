import {
    Box,
    Flex,
    Stack,
    Text,
    Heading,
    Image,
    Card,
    CardBody,

} from "@chakra-ui/react";

export default function WelcomeCard({ welcome }) {
    const date = new Date(welcome.createTime).toLocaleDateString()


    return (
        <div>
            <Card align='center' w='750px' h='auto' bg="transparent" boxShadow="none">
                <CardBody>
                    <Flex w='700px' >
                        <Image borderRadius="full" boxSize="40px" src="/userIcon.png" alt="User Icon" />
                        <Box w="4" />
                        <Stack>
                            <Heading fontSize='md' >{welcome.username}</Heading>
                            <Text fontSize='md' >{welcome.offerDescription}</Text>
                            <Text fontSize='sm' color='grey' >{date}</Text>
                        </Stack>
                    </Flex>

                </CardBody>
            </Card>
        </div>
    )
}   